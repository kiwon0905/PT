  #pragma once

#include <vector>
#include <unordered_map>
#include "Shared/Entity.h"
#include "Shared/EntityManager.h"
#include "Server/Peer.h"
#include "Shared/Zombie.h"
#include "Shared/PickUp.h"
#include "Shared/Human.h"
#include "Shared/GameEvent.h"
#include "Shared/GameMap.h"

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

	template <class T>
	T * createEntity(Entity::Type type);
	Entity * getEntity(Entity::ID id);
	void addEntity(Entity::ID id);

	void leave(Peer * p, Game & s);

	void sync(Game & g);
	void step(Game & s, float dt);

	void handlePacket(Game & g, sf::Packet & packet);

	void loadMap(const std::string & s);

private:

	void removeDeadEntities(Game & g);

	std::vector<Entity *> & getEntitiesOfType(Entity::Type t);
	sf::Vector2f mSize;
	std::vector<std::unique_ptr<GameCommand>> mCommands;

	Entity::ID mNextEntityID;
	EntityManager mEntityMgr;
	std::vector<std::vector<Entity *>> mEntitiesByType;


	std::unordered_map<Peer *, Entity::ID> mEntitiesByPeer;

	GameMap mGameMap;
};

template <class T>
T * GameWorld::createEntity(Entity::Type type)
{
	return mEntityMgr.create<T>(mNextEntityID++, type);
}
