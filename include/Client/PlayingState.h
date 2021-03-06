#pragma once

#include "Client/GameState.h"
#include "Client/ClientGameWorld.h"
#include "Client/ClientGame.h"
#include "Client/Player.h"
#include <string>

class Application;
namespace sf
{
	class Packet;
}

class PlayingState : public GameState
{
public:
	PlayingState();
	~PlayingState();

	virtual void onEnter(Application & app);
	virtual void handleEvent(Application & app);
	virtual void step(Application & app);
	virtual void draw(Application & app);
	virtual void onExit(Application & app);
	virtual void onObscure(Application & app);
	virtual void onReveal(Application & app);
private:
	void loadGUI(Application & app);
	void clearGUI(Application & app);

	void handlePackets(Application & app);
	void handlePacket(Application & app, sf::Packet & packet);

	Game mGame;
};

