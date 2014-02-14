#include "Server/GameWorld.h"
#include "Shared/ValueParser.h"
#include "Shared/Wall.h"


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

void GameWorld::reset()
{
	mEntitiesByType.clear();
	mEntityMgr.destroyAll();
}

bool GameWorld::loadFromFile(const std::string & s)
{
	//Open file
	ValueParser parser;

	if (!parser.loadFromFile(s))
		return false;

	// get size
	if (!parser.get("x", mSize.x))
		return false;
	if (!parser.get("y", mSize.y))
		return false;

	//Load wall data
	int wallCount = 0;
	if (!parser.get("wallCount", wallCount))
		return false;
	
	for (int i = 0; i < wallCount; ++i)
	{
		float x, y, width, height;
		if (!parser.get("wall" + std::to_string(i) + "X", x))
			return false;

		if (!parser.get("wall" + std::to_string(i) + "Y", y))
			return false;

		if (!parser.get("wall" + std::to_string(i) + "Width", width))
			return false;

		if (!parser.get("wall" + std::to_string(i) + "Height", height))
			return false;

		Wall * wall = static_cast<Wall*>(createEntity(Entity::Type::Wall));
		wall->setPosition({ x, y });
		wall->setSize({ width, height });

		addEntity(wall->getID());
	}



}