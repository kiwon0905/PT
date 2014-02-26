#include "Client/GameWorld.h"
#include "Shared/ValueParser.h"
#include "Shared/Wall.h"

#include "Client/Application.h"

GameWorld::GameWorld() 
{
	mEntitiesByType.resize(static_cast<std::size_t>(Entity::Type::Count));
}


GameWorld::~GameWorld()
{
}

void GameWorld::initialize(Textures & textures, sf::RenderTarget & target)
{
	mDrawer.setTarget(target);
	mDrawer.setTextures(textures);

	textures.get("assets/background.png")->setRepeated(true);
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
	

	return true;
}

const std::vector<Entity * > & GameWorld::getEntitiesOfType(Entity::Type t) const
{
	return mEntitiesByType.at(static_cast<std::size_t>(t));
}

void GameWorld::step(float dt)
{
	Entity * player =getEntity(mPlayerEntity);
	if (player)
		player->update(dt);
	mDrawer.update(dt);
}

void GameWorld::draw()
{
	mDrawer.drawTexture("assets/background.png");

	for (Entity * e : getEntitiesOfType(Entity::Type::Wall))
		mDrawer.drawWall(static_cast<Wall &>(*e));
	for (Entity * e : getEntitiesOfType(Entity::Type::PickUp))
		mDrawer.drawPickUp(static_cast<PickUp &>(*e));
	for (Entity * e : getEntitiesOfType(Entity::Type::Human))
		mDrawer.drawHuman(static_cast<Human &>(*e));
	for (Entity * e : getEntitiesOfType(Entity::Type::Zombie))
		mDrawer.drawZombie(static_cast<Zombie &>(*e));
	
	Entity * player = getEntity(mPlayerEntity);
	if (player)
	{
		if (player->getType() == Entity::Type::Human)
			mDrawer.drawHuman(static_cast<Human &>(*player));
		else if (player->getType() == Entity::Type::Zombie)
			mDrawer.drawZombie(static_cast<Zombie &>(*player));
	}

}

void GameWorld::setPlayerEntity(Entity::ID id)
{
	mPlayerEntity = id;
}