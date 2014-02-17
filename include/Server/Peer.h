#pragma once
#include <string>
#include <SFML/Network.hpp>
#include <memory>

class Peer
{
public:
	typedef std::unique_ptr<Peer> Ptr;
	friend class Server;
	Peer();
	~Peer();

	const std::string & getName() const;
	void setName(const std::string & s);

	sf::Socket::Status send(sf::Packet & packet);
	sf::Socket::Status receive(sf::Packet & packet);

	void disconnect();
	bool isDisconnected() const;

	void setReady(bool ready);
	bool isReady() const;
private:
	sf::TcpSocket mSocket;
	std::string mName;
	bool mDisconnected;
	bool mReady;
};

