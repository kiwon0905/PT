#pragma once

#include <vector>
#include "Server/Peer.h"
#include "Server/GameWorld.h"

class Server
{
public:
	enum { nMaxPlayer = 8 };
	const sf::Time TimeStep;
	const sf::Time TimeSync;
	Server();
	~Server();

	void run();
private:
	void pushPacket(Peer * p, sf::Packet * newPacket, bool broadcast = false);
	
	bool isReady();

	void step();
	void sync();

	void handleNewConnection();
	void handleDisconnection();
	void handlePackets();
	void handlePacket(Peer & p, sf::Packet & packet);
	
	void onPeerDisconnect(Peer & p);
	void onRequestJoin(Peer & p, sf::Packet & packet);
	void onChat(Peer & p, sf::Packet & packet);

	
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
	std::vector<Peer::Ptr> mPeers;
	sf::TcpListener mListener;
	std::unique_ptr<GameWorld> mGameWorld;

	std::vector<std::string> mMapList;
	void loadMaps();
	const std::string & getRandomMap();
};

