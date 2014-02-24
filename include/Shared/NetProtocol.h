#pragma once
#include <SFML/Network.hpp>

enum class Cl : sf::Int32
{
	RequestJoin,
	Ready,
	Chat, //name, msg

	GameEvent
};

enum class Sv : sf::Int32
{
	Yes,
	No,

	
	ReplyJoin,
	PlayerJoined,
	PlayerDisconnected,
	Chat, 
	GameStarted, //name
	GameMapData,
	PlayersData,
	GameEvent
};

//packet operators for enum
template <typename Enum>
typename std::enable_if<std::is_enum<Enum>::value, sf::Packet&>::type
operator<< (sf::Packet& packet, Enum t)
{
	return packet << static_cast<typename std::underlying_type<Enum>::type>(t);
}

template <typename Enum>
typename std::enable_if<std::is_enum<Enum>::value, sf::Packet&>::type
operator>> (sf::Packet& packet, Enum & t)
{
	typename std::underlying_type<Enum>::type i;
	packet >> i;
	t = static_cast<Enum>(i);
	return packet;
}