#pragma once
#include <vector>

#include "Shared/Entity.h"
#include "Shared/EntityManager.h"

//Client
class GameWorld
{
public:
	GameWorld();
	~GameWorld();
	
	Entity * createEntity(Entity::ID id, Entity::Type type);
	Entity * getEntity(Entity::ID id);
	void addEntity(Entity::ID id);
	
	bool loadFromFile(const std::string & s);
	void reset();

private:

	EntityManager mEntityMgr;
	std::vector<std::vector<Entity *>> mEntitiesByType;


};

