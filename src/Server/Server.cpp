#include "Server/Server.h"
#include "Shared/NetProtocol.h"
#include "Server/Peer.h"
#include <iostream>

#include <winsock.h>
#pragma comment(lib, "wsock32.lib")
#include <fstream>
#include <Thor/Math.hpp>

std::string  getLocalIp()
{
	char szHostName[255];
	gethostname(szHostName, 255);
	struct hostent *host_entry;
	host_entry = gethostbyname(szHostName);
	return inet_ntoa(*(struct in_addr *)*host_entry->h_addr_list);
}

Server::Server() :TimeStep(sf::seconds(1 / 60.f)), TimeSync(sf::seconds(1 / 30.f))
{
	mListener.listen(sf::Socket::AnyPort);
	mListener.setBlocking(false);
	mPeers.push_back(std::unique_ptr<Peer>(new Peer));

	std::cout << "Server IP: " << getLocalIp() << "\n";
	std::cout << "Server Port: " << mListener.getLocalPort() << "\n";

	loadMaps();
	std::cout << "Maps: ";
	for (auto & s : mMapList)
		std::cout << s << ", ";
	std::cout << std::endl;
}

void Server::loadMaps()
{
	std::ifstream fin;
	fin.open("maps/mapList.txt");
	while (fin.good())
	{
		std::string s;
		std::getline(fin, s);
		mMapList.push_back(s);
	}
}
const std::string & Server::getRandomMap()
{
	return mMapList[thor::random(0u, mMapList.size() - 1)];
}

Server::~Server()
{
}

Server::PacketInfo::PacketInfo(Peer * p, sf::Packet * pckt, bool broadcast) : p(p), pckt(pckt), broadcast(broadcast)
{
}

Server::PacketInfo::PacketInfo(PacketInfo && p) : p(p.p), pckt(std::move(p.pckt)), broadcast(p.broadcast)
{
	p.p = nullptr;
	p.pckt = nullptr;
}
void Server::pushPacket(Peer * p, sf::Packet * newPacket, bool broadcast)
{
	mPackets.emplace_back(p, newPacket, broadcast);
}

void Server::handleNewConnection()
{
	if (mPeers.size() < nMaxPlayer && !mGameWorld)
	{
		if (sf::Socket::Done == mListener.accept(mPeers.back()->mSocket))
		{
			mPeers.push_back(Peer::Ptr(new Peer));
		}
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
			onPeerDisconnect(*mPeers[i]);
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
		onRequestJoin(peer, packet);
		break;
	case Cl::Chat:
		onChat(peer, packet);
		break;
	case Cl::Ready:
		peer.setReady(true);
		std::cout << peer.getName() << " readied\n";
		break;
	default:
		break;
	}
}

void Server::onPeerDisconnect(Peer & p)
{
	sf::Packet * packet = new sf::Packet;
	*packet << Sv::PlayerDisconnected << p.getName();
	pushPacket(&p, packet, true);
}

void Server::onRequestJoin(Peer & peer, sf::Packet & packet)
{
	std::string name;
	packet >> name;
	peer.setName(name);
	std::cout << name << " joined the room\n";
	
	sf::Packet * packet2 =new sf::Packet;//for the peer who joined
	*packet2 << Sv::ReplyJoin << Sv::Yes ;
	pushPacket(&peer, packet2);
	
	sf::Packet * packet3 = new sf::Packet;//for the peer who joined
	sf::Int32 numPlayers = mPeers.size();
	*packet3 << Sv::PlayerJoined << numPlayers -1;
	for (sf::Int32 i = 0; i < numPlayers -1 ; ++i)
	{
		*packet3 << mPeers[i]->getName();
	}

	pushPacket(&peer, packet3);

	sf::Packet * packet4 = new sf::Packet;//for all the others
	*packet4 << Sv::PlayerJoined << sf::Int32(1) << peer.getName();
	pushPacket(&peer, packet4, true);
}

void Server::onChat(Peer & p, sf::Packet & packet)
{
	std::string msg;
	packet >> msg;
	sf::Packet * packet2 = new sf::Packet;
	*packet2 << Sv::Chat << p.getName() << msg;
	pushPacket(nullptr, packet2, true);
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
			step();
		}
		while (elapsedSync >= TimeSync)
		{
			elapsedSync -= TimeSync;
			sync();
		}

	}
}
bool Server::isReady()
{
	if (mPeers.size() < 3)
		return false;
	for (std::size_t i = 0; i < mPeers.size()-1; ++i)
		if (!mPeers[i]->isReady())
			return false;
	return true;
}
void Server::step()
{
	// if game hasn't started yet

	if (!mGameWorld)
	{
		//check if everyone is ready
		if (isReady())
		{
			mGameWorld.reset(new GameWorld);
			std::string mapName = getRandomMap();
			if (!mGameWorld->loadFromFile(mapName))
				std::cout << "failed to load " << mapName << "\n";

			sf::Packet * packet = new sf::Packet;
			*packet << Sv::GameStarted;
			pushPacket(nullptr, packet, true);

			for (auto & p : mPeers)
				p->setReady(false);
		}

	}
}

void Server::sync()
{
	//Flush pending packets;
	for (auto & packetInfo : mPackets)
	{
		if (packetInfo.broadcast)
		{
			for (auto & peer : mPeers)
				if (peer.get() != packetInfo.p)
					peer->send(*packetInfo.pckt);
		}
		else
			packetInfo.p->send(*packetInfo.pckt);
	}
	mPackets.clear();
}