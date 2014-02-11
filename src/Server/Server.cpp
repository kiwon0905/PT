#include "Server/Server.h"
#include "Shared/NetProtocol.h"
#include "Server/Peer.h"
#include <iostream>

#include <winsock.h>
#pragma comment(lib, "wsock32.lib")

std::string  getLocalIp()
{
	char szHostName[255];
	gethostname(szHostName, 255);
	struct hostent *host_entry;
	host_entry = gethostbyname(szHostName);
	return inet_ntoa(*(struct in_addr *)*host_entry->h_addr_list);
}



Server::Server() :TimeStep(sf::seconds(1 / 60.f)), TimeSync(sf::seconds(1/30.f))
{
	mListener.listen(sf::Socket::AnyPort);
	mListener.setBlocking(false);
	mPeers.push_back(std::unique_ptr<Peer>(new Peer));

	std::cout << "Server IP: " << getLocalIp() << "\n";
	std::cout << "Server Port: " << mListener.getLocalPort() << "\n";
}


Server::~Server()
{
}

void Server::handleNewConnection()
{
	if (sf::Socket::Done == mListener.accept(mPeers.back()->mSocket))
	{
		mPeers.push_back(Peer::Ptr(new Peer));
		std::cout << "new peer\n";
	}
}

void Server::handleDisconnection()
{
	auto disconneced = [](const Peer::Ptr & p)
	{
		return p->isDisconnected();
	};
	mPeers.erase(std::remove_if(mPeers.begin(), mPeers.end(), disconneced), mPeers.end());
}
void Server::handlePackets()
{
	for (std::size_t i = 0; i < mPeers.size() - 1; ++i)
	{
		sf::Packet packet;
		sf::Socket::Status s = mPeers[i]->receive(packet);
		while (s == sf::Socket::Done)
		{
			handlePacket(*mPeers[i], packet);
			packet.clear();
			s = mPeers[i]->receive(packet);
		}
		if (s == sf::Socket::Disconnected || s == sf::Socket::Error)
		{
			mPeers[i]->disconnect();
			std::cout << mPeers[i]->getName() << " disconneced\n";
		}

	}
}
void Server::handlePacket(Peer & peer, sf::Packet & packet)
{
	Cl type;
	packet >> type;
	switch (type)
	{
	case Cl::RequestJoin:
	{
		std::string name;
		packet >> name;
		peer.setName(name);
		std::cout << name << " joined the room\n";
		sf::Packet packet2;
		packet2 << Sv::ReplyJoin << Sv::Yes;
		peer.send(packet2);
	}
		break;
	default:
		break;
	}
}

void Server::run()
{
	sf::Clock stepClock, syncClock;
	sf::Time elapsedStep = sf::Time::Zero, elapsedSync = sf::Time::Zero;
	
	while (true)
	{
		handleNewConnection();
		handleDisconnection();
		handlePackets();


		elapsedStep += stepClock.restart();
		elapsedSync += syncClock.restart();


		while (elapsedStep >= TimeStep)
		{
			elapsedStep -= TimeStep;
		}
		while (elapsedSync >= TimeSync)
		{
			elapsedSync -= TimeSync;
		}

	}
}