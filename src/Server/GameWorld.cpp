#include "Server/GameWorld.h"
#include "Shared/ValueParser.h"
#include "Shared/Wall.h"
#include <iostream>
#include "Server/Server.h"
#include "Shared/NetProtocol.h"


GameWorld::GameWorld() : mState(State::Waiting)
{
	mEntitiesByType.resize(static_cast<std::size_t>(Entity::Type::Count));
}


GameWorld::~GameWorld()
{
}

Entity * GameWorld::createEntity(Entity::Type type)
{
	return mEntityMgr.create(mNextEntityID++, type);
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


bool GameWorld::loadFromFile(const std::string & s)
{
	//Open file
	ValueParser parser;
	

	if (!parser.loadFromFile(s))
		return false;
	parser.printAll();
	// get size
	if (!parser.get("X", mSize.x))
		return false;


	if (!parser.get("Y", mSize.y))
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

		Wall * wall = static_cast<Wall*>(createEntity(Entity::Type::Wall));
		wall->setPosition({ x, y });
		wall->setSize({ width, height });

		addEntity(wall->getID());
	}

	std::cout << "Wall count: " << mEntitiesByType.at(static_cast<std::size_t>(Entity::Type::Wall)).size() << "\n";






	return true;
}

void GameWorld::initializePeers(Server & s)
{
	//send map name
	sf::Packet * packet = new sf::Packet;
	*packet << Sv::GameMapData << mMapName;


	std::vector<Entity *> & walls = mEntitiesByType.at(static_cast<std::size_t>(Entity::Type::Wall);
	sf::Int32 wallCount = walls.size();
	*packet << wallCount;

	for (Entity * e : walls)
	{
		//id, x, y, width, height
		*packet << e->getID() << e->getPosition().x << e->getPosition().y << e->getSize().x << e->getSize().y;
	}
	
	sf::Int32 pickUpCount = static_cast<sf::Int32>(mEntitiesByType.at(static_cast<std::size_t>(Entity::Type::PickUp)).size());
	sf::Int32 wallCount = static_cast<sf::Int32>(mEntitiesByType.at(static_cast<std::size_t>(Entity::Type::Wall)).size());
	sf::Int32 wallCount = static_cast<sf::Int32>(mEntitiesByType.at(static_cast<std::size_t>(Entity::Type::Wall)).size());
	

	s.pushPacket(nullptr, packet, true);
}

void GameWorld::handlePacket(sf::Packet & packet)
{

}

void GameWorld::step(Server & s)
{
	switch (mState)
	{
	case GameWorld::State::Waiting:
		if (s.playersAreReady()) // players clicked "ready" from lobby state
		{
			s.unreadyPlayers();

			std::cout << "Game has started!\n";
			
			sf::Packet * packet = new sf::Packet;

			*packet << Sv::GameStarted;
			s.pushPacket(nullptr, packet, true);
			mState = State::Loading;
		}
		break;
	case GameWorld::State::Loading:
		//if players are ready to receive data
		if (s.playersAreReady())
		{
			mMapName = s.getRandomMap();
			loadFromFile(mMapName);
			initializePeers(s);
			mState = State::Playing;
		}
		break;
	case GameWorld::State::Playing:
		std::cout << "playing..\n";
		break;
	case GameWorld::State::Ended:
		break;
	default:
		break;
	}
}