#pragma once
#include "GameState.h"
#include "Shared\DynamicEntity.h"

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

	DynamicEntity e;
};

