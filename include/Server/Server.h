#pragma once

#include <vector>
#include "Server/Peer.h"
#include "Server/ServerGameWorld.h"
#include "Server/ServerGame.h"
class Server
{
public:
	const sf::Time TimeStep;
	const sf::Time TimeSync;
	Server();
	~Server();

	void run();

private:
	void handleNewConnection();
	void handleDisconnection();
	void handlePackets();
	void handlePacket(Peer & p, sf::Packet & packet);

	std::vector<Peer::Ptr> mPeers;
	sf::TcpListener mListener;

	Game mGame;
};

