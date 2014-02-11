#pragma once

class Application;

class GameState
{
public:
	GameState()=default;
	virtual ~GameState()=default;

	virtual void onEnter(Application & app)=0;
	virtual void handleEvent(Application & app) = 0;
	virtual void step(Application & app) = 0;
	virtual void draw(Application & app) = 0;
	virtual void onExit(Application & app) = 0;
	virtual void onObscure(Application & app) = 0;
	virtual void onReveal(Application & app) = 0;
};

