#include "Shared/Zombie.h"

const sf::Int32 Zombie::MaxHealth = 100;

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