#pragma once

#include "Client/ClientGameWorld.h"
#include "Client/Player.h"

namespace sf
{
	class Packet;
	class RenderWindow;
}
class Game
{
public:
	Game();
	~Game();

	void start();
	void initialize(Textures & textures, sf::RenderWindow & window);
	void onPlayersData(sf::Packet & packet);
	void onGameMapData(sf::Packet & packet);
	void handlePacket(sf::Packet & packet);

	void handleEvent(thor::ActionMap<Player::Action> & actions);
	void step(sf::RenderWindow & window, sf::TcpSocket & socket, float dt);
	void draw(Textures & textures, sf::RenderWindow & window);
private:
	enum class State
	{
		Waiting,
		Playing
	} mState;
	GameWorld mGameWorld;
	Player mPlayer;
};

