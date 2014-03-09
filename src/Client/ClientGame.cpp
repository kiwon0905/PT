#include <SFML/Network.hpp>
#include <iostream>

#include "Shared/NetProtocol.h"
#include "Client/ClientGame.h"


Game::Game() :mState(State::Waiting)
{
}


Game::~Game()
{
}

void Game::onPlayersData(sf::Packet & packet)
{
	std::cout << "players data received!\n";
	Entity::Type myType;
	packet >> myType;


	if (myType == Entity::Type::Zombie)
	{
		Entity::ID myID;
		packet >> myID;
		Zombie * me = mGameWorld.createEntity<Zombie>(myID, myType);
		mGameWorld.setPlayerEntity(me->getID());
		mPlayer.setEntity(me);


		sf::Int32 humanCount;
		packet >> humanCount;
		std::cout << "You are the zombie! human count: " << humanCount << std::endl;


		for (sf::Int32 i = 0; i < humanCount; ++i)
		{
			Entity::ID id;
			packet >> id;
			Human * h = mGameWorld.createEntity<Human>(id, Entity::Type::Human);
			mGameWorld.addEntity(h->getID());
		}
	}


	else if (myType == Entity::Type::Human)
	{
		Entity::ID zombieID, myID;
		packet >> zombieID >> myID;
		sf::Int32 playerCount;
		packet >> playerCount;
		std::cout << "Zombie ID: " << zombieID << " Other human count: " << playerCount << std::endl;


		Zombie * z = mGameWorld.createEntity<Zombie>(zombieID, Entity::Type::Zombie);
		mGameWorld.addEntity(z->getID());
		Human * me = mGameWorld.createEntity<Human>(myID, Entity::Type::Human);
		mGameWorld.setPlayerEntity(me->getID());
		mPlayer.setEntity(me);
		//create other players
		for (sf::Int32 i = 0; i < playerCount; ++i)
		{
			Entity::ID humanID;
			packet >> humanID;


			Human * h = mGameWorld.createEntity<Human>(humanID, Entity::Type::Human);
			mGameWorld.addEntity(h->getID());
		}
	}
	else
		assert(false && "something got fucked up");

}

void Game::onGameMapData(sf::Packet & packet)
{
	std::string mapName;
	sf::Int32 wallCount;

	packet >> mapName >> wallCount;
	std::cout << "map data received: " << mapName <<std::endl;
	for (sf::Int32 i = 0; i < wallCount; ++i)
	{
		Entity::ID id;
		float x, y, width, height;
		packet >> id >> x >> y >> width >> height;
		Wall * w = mGameWorld.createEntity<Wall>(id, Entity::Type::Wall);
		w->setPosition({ x, y });
		w->setSize({ width, height });
		mGameWorld.addEntity(id);
	}

	std::cout << "wall count: " << wallCount << std::endl;
	for (auto & wall : mGameWorld.getEntitiesOfType(Entity::Type::Wall))
	{
		std::cout << wall->getID() << ": " << wall->getPosition().x << " " << wall->getPosition().y << " " << wall->getSize().x << " " << wall->getSize().y << std::endl;
	}
}

void Game::handlePacket(sf::Packet & packet)
{
	if (mState == State::Playing)
	{
		mGameWorld.handlePacket(packet);
	}

}

void Game::handleEvent(thor::ActionMap<Player::Action> & actions)
{
	if (mState == State::Playing)
	{
		mPlayer.handleEvent(actions);
	}

}
void Game::step(sf::RenderWindow & window, sf::TcpSocket & socket, float dt)
{
	if (mState == State::Playing)
	{
		mGameWorld.step(dt);
		mPlayer.sync(socket);
		mPlayer.update(window);
	}
}
void Game::draw(Textures & textures, sf::RenderWindow & window)
{
	if (mState == State::Playing)
		mGameWorld.draw(textures, window);
}

void Game::start()
{
	mState = State::Playing;
}