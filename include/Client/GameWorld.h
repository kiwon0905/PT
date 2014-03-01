#pragma once
#include <vector>

#include "Shared/Entity.h"
#include "Shared/EntityManager.h"
#include "Client/Drawer.h"
#include "Shared/GameEvent.h"

namespace sf
{
	class Packet;
}
class Application;
//Client
class GameWorld
{
public:
	GameWorld();
	~GameWorld();
	
	void initialize(Textures & textures, sf::RenderTarget & target);

	template <class T>
	T * createEntity(Entity::ID id, Entity::Type type);
	Entity * getEntity(Entity::ID id);
	void addEntity(Entity::ID id);
	
	bool loadFromFile(const std::string & s);
	void reset();

	void step(float dt);
	void draw(sf::RenderWindow & window);

	void setPlayerEntity(Entity::ID id);

	void handlePacket(sf::Packet & packet);

private:
	std::vector<Entity * > & getEntitiesOfType(Entity::Type t);
	std::vector<std::unique_ptr<GameCommand>> mCommands;
	void removeDeadEntities();
	
	Drawer mDrawer;
	EntityManager mEntityMgr;
	std::vector<std::vector<Entity *>> mEntitiesByType;
	Entity::ID mPlayerEntity;
};

template <class T>
T * GameWorld::createEntity(Entity::ID id, Entity::Type type)
{
	return mEntityMgr.create<T>(id, type);
}