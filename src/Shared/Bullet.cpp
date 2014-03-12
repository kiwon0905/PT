#include "Shared/Bullet.h"
#include <Thor/Vectors.hpp> 

Bullet::Bullet(Entity::ID id) : DynamicEntity(id)
{
	setType(Entity::Type::Bullet);
	setSize({ 5.f, 5.f });
	setAcceleration(3000.f);
	setMaxSpeed(800);
}

void Bullet::setDirection(float angle)
{
	setRotation(angle);
	setGoalVelocity({ 400.f, 0 });
}


Bullet::~Bullet()
{
}