#include "Server/GameWorld.h"
#include "Server/Game.h"
#include "Shared/ValueParser.h"
#include "Shared/Wall.h"
#include "Shared/NetProtocol.h"
#include "Shared/GameEvent.h"

#include <Thor/Math.hpp>
#include <iostream>

GameWorld::GameWorld()
{
	mEntitiesByType.resize(static_cast<std::size_t>(Entity::Type::Count));
}


GameWorld::~GameWorld()
{
}

Entity * GameWorld::getEntity(Entity::ID id)
{
	return mEntityMgr.get(id);
}

void GameWorld::addEntity(Entity::ID id)
{
	Entity * e = getEntity(id);
	mEntitiesByType.at(static_cast<std::size_t>(e->getType())).push_back(e);
}

std::vector<Entity *> & GameWorld::getEntitiesOfType(Entity::Type t)
{
	return mEntitiesByType.at(static_cast<std::size_t>(t));
}

bool GameWorld::loadFromFile(const std::string & s)
{
	//Open file
	ValueParser parser;
	

	if (!parser.loadFromFile(s))
		return false;
	parser.printAll();
	mMapName = s;
	// get size
	if (!parser.get("SizeX", mSize.x))
		return false;

	if (!parser.get("SizeY", mSize.y))
		return false;

	//Load wall data
	int wallCount = 0;
	if (!parser.get("WallCount", wallCount))
		return false;
	
	for (int i = 0; i < wallCount; ++i)
	{
		float x, y, width, height;
		if (!parser.get("Wall" + std::to_string(i) + "X", x))
			return false;

		if (!parser.get("Wall" + std::to_string(i) + "Y", y))
			return false;

		if (!parser.get("Wall" + std::to_string(i) + "Width", width))
			return false;

		if (!parser.get("Wall" + std::to_string(i) + "Height", height))
			return false;

		Wall * wall = createEntity<Wall>(Entity::Type::Wall);
		wall->setPosition({ x, y });
		wall->setSize({ width, height });

		addEntity(wall->getID());
	}

	std::cout << "Wall count: " << sf::Int32(mEntitiesByType.at(static_cast<std::size_t>(Entity::Type::Wall)).size()) << "\n";
	return true;
}

void GameWorld::leave(Peer * p, Game & game)
{
	//if this peer has an entity associated with it, kill it
	if (mEntitiesByPeer.count(p) == 1)
		getEntity(mEntitiesByPeer[p])->kill();
	
	sf::Packet * packet = new sf::Packet;
	*packet << Sv::GameEvent << GameEvent::DestroyEntity << mEntitiesByPeer[p];
	game.pushPacket(p, packet, true);

	mEntitiesByPeer.erase(p);
}

void GameWorld::removeDeadEntities()
{
	//remove dead entities
	auto isDead = [this](Entity * e)
	{
		if (!e->isAlive())
		{
			mEntityMgr.destroy(e->getID());
			return true;
		}
		return false;
	};
	auto & zombies = getEntitiesOfType(Entity::Type::Zombie);
	zombies.erase(std::remove_if(zombies.begin(), zombies.end(), isDead), zombies.end());
	auto & humans = getEntitiesOfType(Entity::Type::Human);
	humans.erase(std::remove_if(humans.begin(), humans.end(), isDead), humans.end());
}

void GameWorld::step(Game & game)
{
	removeDeadEntities();
	for (auto & command : mCommands)
		(*command)();
	mCommands.clear();
}

void GameWorld::sync(Game & game)
{
	
	//position
	sf::Packet * packet = new sf::Packet;
	*packet << Sv::GameEvent << GameEvent::MoveEntity << sf::Int32(getEntitiesOfType(Entity::Type::Zombie).size());
	for (Entity * e : getEntitiesOfType(Entity::Type::Zombie))
		*packet << e->getID() <<e->getPosition().x << e->getPosition().y;
	game.pushPacket(nullptr, packet, true);
	
	sf::Packet * packet2 = new sf::Packet;
	*packet2 << Sv::GameEvent << GameEvent::MoveEntity << sf::Int32(getEntitiesOfType(Entity::Type::Human).size());
	for (Entity * e : getEntitiesOfType(Entity::Type::Human))
		*packet2 << e->getID() << e->getPosition().x << e->getPosition().y;
	game.pushPacket(nullptr, packet2, true);

	sf::Packet * packet3 = new sf::Packet;
	*packet3 << Sv::GameEvent << GameEvent::RotateEntity << sf::Int32(getEntitiesOfType(Entity::Type::Zombie).size());
	for (Entity * e : getEntitiesOfType(Entity::Type::Zombie))
		*packet3 << e->getID() << static_cast<DynamicEntity*>(e)->getRotation();
	game.pushPacket(nullptr, packet3, true);

	sf::Packet * packet4 = new sf::Packet;
	*packet4 << Sv::GameEvent << GameEvent::RotateEntity << sf::Int32(getEntitiesOfType(Entity::Type::Human).size());
	for (Entity * e : getEntitiesOfType(Entity::Type::Human))
		*packet4 << e->getID() << static_cast<DynamicEntity*>(e)->getRotation();
	game.pushPacket(nullptr, packet4, true);
	
}


void GameWorld::handlePacket(sf::Packet & packet)
{
	GameEvent ev;
	packet >> ev;
	switch (ev)
	{
	case GameEvent::MoveEntity:
	{
		Entity::ID id;
		packet >> id;
		Entity * e = getEntity(id);
		if (e)
		{
			float x, y;
			packet >> x >> y;
			MoveEntity * move = new MoveEntity;
			move->e = e;
			move->x = x;
			move->y = y;
			mCommands.emplace_back(move);
		}
		break;
	}
	case GameEvent::RotateEntity:
	{
		Entity::ID id;
		packet >> id;
		Entity * e = getEntity(id);
		if (e)
		{
			float angle;
			packet >> angle;
			RotateEntity * rotate = new RotateEntity;
			rotate->e = static_cast<DynamicEntity *>(e);
			rotate->a = angle;
			mCommands.emplace_back(rotate);
			std::cout << "Rotate " << id << " " << angle << " degrees " << std::endl;
		}
	}
	break;
	case GameEvent::DestroyEntity:
		break;
	default:
		break;
	}
}