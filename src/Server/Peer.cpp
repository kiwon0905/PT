#include "Server/Peer.h"


Peer::Peer() :mDisconnected(false)
{
	mSocket.setBlocking(false);
}


Peer::~Peer()
{
}

const std::string & Peer::getName() const
{
	return mName;
}
void Peer::setName(const std::string & s)
{
	mName = s;
}

sf::Socket::Status Peer::send(sf::Packet & packet)
{
	return mSocket.send(packet);
}
sf::Socket::Status Peer::receive(sf::Packet & packet)
{
	return mSocket.receive(packet);
}

void Peer::disconnect()
{
	mDisconnected = true;
}
bool Peer::isDisconnected() const
{
	return mDisconnected;
}
