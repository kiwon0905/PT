#include "Shared/Bullet.h"
#include <Thor/Vectors.hpp> 

const sf::Int32 Bullet::Damage = 10;

Bullet::Bullet(Entity::ID id) : DynamicEntity(id)
{
	setType(Entity::Type::Bullet);
	setSize({ 5.f, 5.f });
	setAcceleration(50000000.f);
	setMaxSpeed(400);	
	
	
}

void Bullet::setDirection(float angle)
{
	setRotation(angle);
	setGoalVelocity(thor::PolarVector2f(400.f, angle));
}


Bullet::~Bullet()
{
}
