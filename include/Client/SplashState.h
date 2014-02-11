#pragma once
#include "Client/GameState.h"
#include "Client/Player.h"
#include <SFML/Network.hpp>
#include <Thor/Input.hpp>

class SplashState : public GameState
{
public:
	SplashState();
	virtual ~SplashState();

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
	void initActions(thor::ActionMap<Player::Action> & actions);


	void connect(Application & app);
};