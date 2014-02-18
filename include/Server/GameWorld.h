#pragma once

#include <vector>
#include "Shared/Entity.h"
#include "Shared/EntityManager.h"

class Server;

namespace sf
{
	class Packet;
}
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

	void initializePeers(Server & s);

	void handlePacket(sf::Packet & packet);
	void step(Server & s);
private:
	enum class State
	{
		Waiting,
		Loading,
		Playing,
		Ended
	} mState;
	sf::Vector2f mSize;
	Entity::ID mNextEntityID;
	EntityManager mEntityMgr;
	std::vector<std::vector<Entity *>> mEntitiesByType;

	std::string mMapName;

};

