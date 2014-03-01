#include "Client/GameWorld.h"
#include "Shared/ValueParser.h"
#include "Shared/Wall.h"
#include "Shared/GameEvent.h"
#include "Client/Application.h"
#include "Shared/NetProtocol.h"

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

std::vector<Entity * > & GameWorld::getEntitiesOfType(Entity::Type t)
{
	return mEntitiesByType.at(static_cast<std::size_t>(t));
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

void GameWorld::step(float dt)
{
	removeDeadEntities();

	for (auto & command : mCommands)
		(*command)();
	mCommands.clear();
	
	Entity * player =getEntity(mPlayerEntity);
	if (player)
		player->update(dt);


	mDrawer.update(dt);
}

void GameWorld::draw(sf::RenderWindow & window)
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

void GameWorld::handlePacket(sf::Packet & packet)
{
	GameEvent ev;
	packet >> ev;
	switch (ev)
	{
	case GameEvent::MoveEntity:
	{
		sf::Int32 num;
		packet >> num;
		for (sf::Int32 i = 0; i < num; ++i)
		{
			Entity::ID id;
			packet >> id;
			Entity * e = getEntity(id);

			float x, y;
			packet >> x >> y;
			//if (id != mPlayerEntity)
			{
				MoveEntity * move = new MoveEntity;
				move->e = e;
				move->x = x;
				move->y = y;
				//if (e &&( mPlayerEntity = e->getID()))
					mCommands.emplace_back(move);
			}

		}
		
	}
		break;
	case GameEvent::DestroyEntity:
	{
		Entity::ID id;
		packet >> id;
		getEntity(id)->kill();
	}
		break;
	default:
		break;
	}

}