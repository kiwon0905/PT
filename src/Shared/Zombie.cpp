#include "Shared/Zombie.h"
#include <Thor/Vectors.hpp>

const sf::Int32 Zombie::MaxHealth = 100;
const sf::Int32 Zombie::Damage = 20;
const sf::Time Zombie::Skill1CoolDown = sf::seconds(1.25f);
const sf::Time Zombie::Skill1Duration = sf::seconds(.5f);
Zombie::Zombie(Entity::ID id) : DynamicEntity(id), mHealth(MaxHealth)
{
	setType(Entity::Type::Zombie);
	setSize({ 50.f, 50.f });
	setAcceleration(4000.f);
	setMaxSpeed(110.f);
}


Zombie::~Zombie()
{
}


void Zombie::takeDamage(sf::Int32 amount)
{
	mHealth -= amount;
	if (mHealth < 0)
		kill();
}

sf::FloatRect Zombie::getMeeleAttackBox() const
{
	float angle = getRotation();
	
	sf::Vector2f center = getCenter() + sf::Vector2f(thor::PolarVector2f(50, angle));
	return sf::FloatRect(center.x - 25, center.y - 25, 50, 50);

}

bool Zombie::skill1()
{
	if (mSkill1LastUsed.getElapsedTime() >Skill1CoolDown)
	{
		mSkill1LastUsed.restart();
		return true;
	}
	return false;

}
bool Zombie::isSkill1Active() const
{
	return	mSkill1LastUsed.getElapsedTime() < Skill1Duration;

}