#pragma once

#include <vector>
#include "Shared/Entity.h"
#include "Shared/EntityManager.h"

class Game;

namespace sf
{
	class Packet;
}
//Server
class GameWorld
{
	friend class Game;
public:
	GameWorld();
	~GameWorld();

	Entity * createEntity(Entity::Type type);
	Entity * getEntity(Entity::ID id);
	void addEntity(Entity::ID id);

	bool loadFromFile(const std::string & s);

	void step(Game & s);
private:
	const std::vector<Entity *> & getEntitiesOfType(Entity::Type t) const;
	sf::Vector2f mSize;

	Entity::ID mNextEntityID;
	EntityManager mEntityMgr;
	std::vector<std::vector<Entity *>> mEntitiesByType;

	std::string mMapName;

};

