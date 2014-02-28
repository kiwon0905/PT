#pragma once

#include <unordered_map>
#include "Shared/NetProtocol.h"
#include "Server/GameWorld.h"

class Server;
class Peer;

class Game
{
public:
	Game();
	~Game();

	void step(Server & s);
	void sync(Server & s);

	void join(Server & s, Peer & p);
	void leave(Server & s, Peer & p);
	const std::string & getRandomMap();

	void pushPacket(Peer * p, sf::Packet * newPacket, bool broadcast = false);

	void handlePacket(Peer & p, Cl type, sf::Packet & packet);

private:
	struct PacketInfo
	{
		PacketInfo(Peer * p, sf::Packet * pckt, bool broadcast);
		PacketInfo(PacketInfo && p);
		PacketInfo(const PacketInfo & p) = delete;
		PacketInfo & operator=(const PacketInfo & p) = delete;
		Peer * p;
		bool broadcast;
		std::unique_ptr<sf::Packet> pckt;
	};
	std::vector<PacketInfo> mPackets;

	enum class State
	{
		Waiting, //players are in lobby
		Loading, //players are downloading map data
		Playing, //game is playing
		Ended,
	} mState;

	
	std::unordered_map<Peer *, bool> mPlayers;
	
	bool playersAreReady();
	void unreadyPlayers();

	GameWorld mGameWorld;
	std::vector<std::string> mMapList;
	
	void loadMaps();

};

