#pragma once

class DynamicEntity;
#include <SFML/System.hpp>
namespace sf
{
	class TcpSocket;
	class RenderWindow;
}
namespace thor
{
	template <class T>
	class ActionMap;
}

class Player
{
public:
	Player();
	~Player();
	enum Action
	{
		MoveE,	//0
		MoveN,	//90
		MoveW,	//180
		MoveS,	//270
		Skill1
	};

	sf::Clock mLastSkill1Used;
	bool mSkill1;

	void setEntity(DynamicEntity * e);
	void sync(sf::TcpSocket & socket);
	void update(sf::RenderWindow & window);
	void handleEvent(thor::ActionMap<Player::Action> & mActions);

private:
	DynamicEntity * mEntity;
};

