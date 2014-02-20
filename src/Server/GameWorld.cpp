#include "Server/GameWorld.h"
#include "Server/Game.h"
#include "Shared/ValueParser.h"
#include "Shared/Wall.h"
#include "Shared/NetProtocol.h"

#include <Thor/Math.hpp>
#include <iostream>

GameWorld::GameWorld()
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

const std::vector<Entity *> & GameWorld::getEntitiesOfType(Entity::Type t) const
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



void GameWorld::step(Game & game)
{
}