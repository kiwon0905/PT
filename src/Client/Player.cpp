#include "Client/Player.h"
#include <SFML/Network.hpp>
#include <Thor/Input.hpp>
#include <Thor/Vectors.hpp>
#include "Shared/DynamicEntity.h"

Player::Player() 
{
}


Player::~Player()
{
}

void Player::setEntity(DynamicEntity * e)
{
	mEntity = e;
}

void Player::sync(sf::TcpSocket & socket)
{

}
void Player::handleEvent(thor::ActionMap<Player::Action> & mActions)
{
	if (mEntity)
	{
		sf::Vector2f vel;

	if (mActions.isActive(Player::Action::MoveE))
		vel += sf::Vector2f(300, 0);
	if (mActions.isActive(Player::Action::MoveW))
		vel += sf::Vector2f(-300, 0);
	if (mActions.isActive(Player::Action::MoveN))
		vel += sf::Vector2f(0, -300);
	if (mActions.isActive(Player::Action::MoveS))
		vel += sf::Vector2f(0, 300);

	mEntity->setGoalVelocity(vel);
	}
	
}
