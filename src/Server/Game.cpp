#include "Server/Game.h"
#include "Server/Server.h"
#include "Shared/NetProtocol.h"
#include <Thor/Math.hpp>
#include <iostream>
#include <fstream>



Game::Game() : mState(State::Waiting)
{
	loadMaps();
}


Game::~Game()
{
}

Game::PacketInfo::PacketInfo(Peer * p, sf::Packet * pckt, bool broadcast) : p(p), pckt(pckt), broadcast(broadcast)
{
}

Game::PacketInfo::PacketInfo(PacketInfo && p) : p(p.p), pckt(std::move(p.pckt)), broadcast(p.broadcast)
{
	p.p = nullptr;
	p.pckt = nullptr;
}

void Game::pushPacket(Peer * p, sf::Packet * newPacket, bool broadcast)
{
	mPackets.push_back(PacketInfo(p, newPacket, broadcast));
}

void Game::join(Server & s, Peer & peer)
{

	sf::Packet * packet2 = new sf::Packet;//for the peer who joined. join success
	*packet2 << Sv::ReplyJoin << Sv::Yes;
	pushPacket(&peer, packet2);

	sf::Packet * packet4 = new sf::Packet;//for all the others
	*packet4 << Sv::PlayerJoined << sf::Int32(1) << peer.getName();
	pushPacket(&peer, packet4, true);

	sf::Packet * packet3 = new sf::Packet;
	*packet3 << Sv::PlayerJoined << sf::Int32(mPlayers.size());
	for (auto & player : mPlayers)
		*packet3 << player.first->getName();
	pushPacket(&peer, packet3);
	mPlayers.insert({ &peer, false });
}

void Game::leave(Server & s, Peer & p)
{
	mPlayers.erase(&p);
	sf::Packet * packet = new sf::Packet;
	*packet << Sv::PlayerDisconnected << p.getName() << "\n";
	pushPacket(nullptr, packet, true);
	mGameWorld.leave(&p, *this);
}

bool Game::playersAreReady()
{
	if (mPlayers.size() == 0)
		return false;
	for (auto & players : mPlayers)
		if (!players.second)
			return false;

	return true;
}
void Game::unreadyPlayers()
{
	for (auto & players : mPlayers)
		players.second = false;
}

void Game::step(Server & s)
{
	switch (mState)
	{
	case Game::State::Waiting:// players clicked "ready" from lobby
		if (playersAreReady())
		{
			sf::Packet * packet = new sf::Packet;
			*packet << Sv::GameStarted;
			pushPacket(nullptr, packet, true);
			
			unreadyPlayers(); //unready them so they can be ready to receive map data
			std::cout << "Game is starting!" << std::endl;
			mState = Game::State::Loading;
		}
		break;
	case Game::State::Loading:
		if (playersAreReady())
		{
			std::string mapName = getRandomMap();
			mGameWorld.loadFromFile(mapName);
			sf::Packet * packet = new sf::Packet;
			*packet << Sv::GameMapData << mapName;

			const std::vector<Entity *> & walls = mGameWorld.getEntitiesOfType(Entity::Type::Wall);
			*packet << sf::Int32(walls.size());
			for (Entity * e : walls)
			{
				*packet << e->getID();
				*packet << e->getPosition().x << e->getPosition().y << e->getSize().x << e->getSize().y;
			}

			pushPacket(nullptr, packet, true);
			mState = Game::State::Playing;
			mGameWorld.init(*this, mPlayers.begin(), mPlayers.end());
		}

		break;
	case Game::State::Playing:
		mGameWorld.step(*this);
		break;
	case Game::State::Ended:
		break;
	default:
		break;
	}
}

void Game::sync(Server & s)
{
	if (mState == State::Playing)
		mGameWorld.sync(*this);
		//flush the pending packets
	
	for (auto & packetInfo : mPackets)
	{
		if (packetInfo.broadcast)
		{
			for (auto & player : mPlayers)
			{
				if (player.first != packetInfo.p)
					player.first->send(*packetInfo.pckt);
			}
		}
		else
			packetInfo.p->send(*packetInfo.pckt);
	}
	mPackets.clear();
}

void Game::handlePacket(Peer & peer, Cl type, sf::Packet & packet)
{
	switch (type)
	{
	case Cl::Ready:
		std::cout << peer.getName() << " readied\n";
		mPlayers.at(&peer) = true;
		break;
	case Cl::Chat:
	{
		std::string msg;
		packet >> msg;
		sf::Packet * packet2 = new sf::Packet;
		*packet2<<Sv::Chat << peer.getName() << msg;
		pushPacket(nullptr, packet2, true);
	}
		break;
	case Cl::GameEvent:
		mGameWorld.handlePacket(packet);
		break;
	default:
		break;
	}
}
void Game::loadMaps()
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

const std::string & Game::getRandomMap()
{
	return mMapList[thor::random(0u, mMapList.size() - 1)];
}
