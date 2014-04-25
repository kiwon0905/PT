#include "Client/Player.h"
#include <SFML/Network.hpp>
#include <Thor/Input.hpp>
#include <Thor/Vectors.hpp>
#include "Shared/DynamicEntity.h"
#include "Shared/Human.h"
#include "Shared/Zombie.h"
#include "Shared/NetProtocol.h"
#include "Shared/GameEvent.h"
#include <cmath>
#include <iostream>

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
	
		if (mSkill1)
		{
			if (mEntity->getType() == Entity::Type::Human)
			{
				sf::Packet packet;
				packet << Cl::GameEvent << GameEvent::ShootBullet <<mEntity->getID() << mEntity->getRotation() << mEntity->getCenter().x << mEntity->getCenter().y;
				socket.send(packet);
			}
			else if (mEntity->getType() == Entity::Type::Zombie)
			{
				std::cout << "Zombie melee attack\n";
				
				sf::Packet packet;
				packet << Cl::GameEvent << GameEvent::ZombieMeleeAttack;
				socket.send(packet);
			
			}
		}
	}

}

#include <iostream>
void Player::update(sf::RenderWindow & window)
{
	

}

void Player::handleRealtimeEvent(sf::RenderWindow & window, thor::ActionMap<Player::Action> & actions)
{
	float angle;
	if (mEntity)
	{
		sf::Vector2f mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
		mousePos = window.mapPixelToCoords(sf::Vector2i(mousePos));
		sf::Vector2f center = mEntity->getCenter();
		sf::Vector2f d = mousePos - center;

		angle = thor::toDegree(std::atan2f(d.y, d.x));
		mEntity->setRotation(angle);
	}

	if (mEntity)
	{
		sf::Vector2f vel;

		if (actions.isActive(Player::Action::MoveE))
			vel += sf::Vector2f(300, 0);
		if (actions.isActive(Player::Action::MoveW))
			vel += sf::Vector2f(-300, 0);
		if (actions.isActive(Player::Action::MoveN))
			vel += sf::Vector2f(0, -300);
		if (actions.isActive(Player::Action::MoveS))
			vel += sf::Vector2f(0, 300);

		mEntity->setGoalVelocity(vel);
	
		if (mEntity->getType() == Entity::Type::Zombie)
		{
			Zombie * z = static_cast<Zombie*>(mEntity);
			mSkill1 = actions.isActive(Player::Action::Skill1) && z->skill1();
		
		}
		else if (mEntity->getType() == Entity::Type::Human)
		{
			Human * h = static_cast<Human*>(mEntity);
			mSkill1 = actions.isActive(Player::Action::Skill1) && h->skill1();

		}
		
	}
}

void Player::handleEvent(thor::ActionMap<Player::Action> & actions)
{


	
	
}


void Player::draw(sf::RenderWindow  & window)
{
	/*if (mEntity)
	{
		if (mEntity->getType() == Entity::Type::Zombie && static_cast<Zombie*>(mEntity)->isSkill1Active())
		{
			sf::RectangleShape r;
			sf::FloatRect aabb = static_cast<Zombie*>(mEntity)->getMeeleAttackBox();
			r.setPosition(aabb.left, aabb.top);
			r.setSize({ aabb.width, aabb.height });
			r.setFillColor(sf::Color::White);
			window.draw(r);
		}
	}*/
	
}