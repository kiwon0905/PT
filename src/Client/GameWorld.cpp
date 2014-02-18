#include "Client/GameWorld.h"
#include "Shared/ValueParser.h"
#include "Shared/Wall.h"



GameWorld::GameWorld()
{
	mEntitiesByType.resize(static_cast<std::size_t>(Entity::Type::Count));
}


GameWorld::~GameWorld()
{
}

Entity * GameWorld::createEntity(Entity::ID id, Entity::Type type)
{
	return mEntityMgr.create(id, type);
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
	ValueParser parser;
	parser.loadFromFile(s);

	
	std::string backgroundTexture;
	if (parser.get("BackgroundTexture", backgroundTexture))
		return false;
}