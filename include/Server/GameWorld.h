  #pragma once

#include <vector>
#include <unordered_map>
#include "Shared/Entity.h"
#include "Shared/EntityManager.h"
#include "Server/Peer.h"
#include "Shared/Zombie.h"
#include "Shared/PickUp.h"
#include "Shared/Human.h"

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

	bool loadFromFile(const std::string & s);

	void leave(Peer * p, Game & s);
	void step(Game & s);
private:
	template <class Iterator>
	void init(Game & g, Iterator & begin, Iterator & end); //called by Game when the game starts
	void removeDeadEntities();

	std::vector<Entity *> & getEntitiesOfType(Entity::Type t);
	sf::Vector2f mSize;

	Entity::ID mNextEntityID;
	EntityManager mEntityMgr;
	std::vector<std::vector<Entity *>> mEntitiesByType;

	std::string mMapName;

	std::unordered_map<Entity::ID, Peer *> mPeersByEntity;
	std::unordered_map<Peer *, Entity::ID> mEntitiesByPeer;
};

template <class T>
T * GameWorld::createEntity(Entity::Type type)
{
	return mEntityMgr.create<T>(mNextEntityID++, type);
}

template <class Iterator>
void GameWorld::init(Game & game, Iterator & begin, Iterator & end)
{
	std::vector<Peer *> players;
	for (auto iter = begin; iter != end; ++iter)
		players.push_back(iter->first);

	//choose a zombie
	Zombie * z = createEntity<Zombie>(Entity::Type::Zombie);
	Peer * zombiePlayer = players[thor::random(0u, players.size() - 1)];
	mPeersByEntity[z->getID()] = zombiePlayer;
	mEntitiesByPeer[zombiePlayer] = z->getID();

	//remove the zombie player from players vector
	players.erase(std::remove(players.begin(), players.end(), zombiePlayer), players.end());

	//rest are human
	for (Peer * human : players)
	{
		Human * h = createEntity<Human>(Entity::Type::Human);
		mPeersByEntity[h->getID()] = human;
		mEntitiesByPeer[human] = h->getID();
	}

	//@zombie
	sf::Packet * packet = new sf::Packet;
	*packet << Sv::PlayersData << Entity::Type::Zombie << z->getID() << sf::Int32(players.size());
	for (Peer * human : players)
	{
		//id, 
		*packet << mEntitiesByPeer[human];
	}
	game.pushPacket(zombiePlayer, packet);


	//@human
	std::cout << "Zombie ID: " << z->getID() << "Human count: " << players.size() << std::endl;
	for (Peer * human : players)
	{
		sf::Packet * packet2 = new sf::Packet;        //      zombie id   player id
		*packet2 << Sv::PlayersData << Entity::Type::Human << z->getID() << mEntitiesByPeer[human] << sf::Int32(players.size() - 1);

		//insert other's players info
		for (Peer * human2 : players)
		{  //other player's id
			if (human2 != human)
				*packet << mEntitiesByPeer[human2];
		}
		game.pushPacket(human, packet2);
	}





}