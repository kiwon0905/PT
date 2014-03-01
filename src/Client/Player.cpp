#include "Client/Player.h"
#include <SFML/Network.hpp>
#include <Thor/Input.hpp>
#include <Thor/Vectors.hpp>
#include "Shared/DynamicEntity.h"
#include "Shared/NetProtocol.h"
#include "Shared/GameEvent.h"
#include <cmath>

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
	if (mEntity)
	{
		sf::Packet packet;
		packet << Cl::GameEvent << GameEvent::MoveEntity << mEntity->getID() << mEntity->getPosition().x << mEntity->getPosition().y;
		socket.send(packet);
		
		packet.clear();
		
		packet << Cl::GameEvent << GameEvent::RotateEntity << mEntity->getID() << mEntity->getRotation();
		socket.send(packet);
	}

}
#include <iostream>
void Player::update(sf::RenderWindow & window)
{
	if (mEntity)
	{
		sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
		sf::Vector2f d = static_cast<sf::Vector2f>(mousePos) - mEntity->getPosition();
		float angle = std::atan2f(d.y, d.x);
		mEntity->setRotation(thor::toDegree(angle));

	}

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
void Player::draw(sf::RenderWindow & window)
{
	if (mEntity)
	{
		sf::View view = window.getDefaultView();
		view.setCenter(mEntity->getPosition() + mEntity->getSize()/2.f);
		window.setView(view);
	}
}