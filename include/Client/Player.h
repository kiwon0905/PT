#pragma once

class DynamicEntity;

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

	void setEntity(DynamicEntity * e);
	void sync(sf::TcpSocket & socket);
	void update(sf::RenderWindow & window);
	void handleEvent(thor::ActionMap<Player::Action> & mActions);
	void draw(sf::RenderWindow & window);
private:
	DynamicEntity * mEntity;
};

