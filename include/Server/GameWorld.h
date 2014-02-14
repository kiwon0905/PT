#pragma once

#include <vector>
#include "Shared/Entity.h"
#include "Shared/EntityManager.h"

//Server
class GameWorld
{
public:
	GameWorld();
	~GameWorld();

	Entity * createEntity(Entity::Type type);
	Entity * getEntity(Entity::ID id);
	void addEntity(Entity::ID id);

	bool loadFromFile(const std::string & s);
	void reset();

private:
	sf::Vector2f mSize;
	Entity::ID mNextEntityID;
	EntityManager mEntityMgr;
	std::vector<std::vector<Entity *>> mEntitiesByType;


};

