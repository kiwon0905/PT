#pragma once
#include "GameState.h"
#include "Shared/DynamicEntity.h"

#include "Client/ChatBox.h"
#include <SFML/Network.hpp>

class LobbyState : public GameState
{
public:
	LobbyState();
	~LobbyState();

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


	void onPlayerJoin(Application & app, sf::Packet & packet);
	void onPlayerDisconnect(Application & app, sf::Packet & packet);
	void onChat(Application & app, sf::Packet & packet);

	ChatBox mChatBox;
};

