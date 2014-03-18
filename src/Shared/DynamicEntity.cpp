#include "Shared/DynamicEntity.h"
#include <Thor/Graphics.hpp>
#include <Thor/Vectors.hpp>
#include <iostream>

DynamicEntity::DynamicEntity(Entity::ID id) :
Entity(id), mGoalVelocity(), mCurrentVelocity(), mAcceleration(), mRotation()
{
}

float DynamicEntity::accelerate(float current, float goal, float dt)
{
	float diff = goal - current;
	if (std::abs(diff) < mAcceleration*dt)
		return goal;
	else if (diff < 0)
		return current -= mAcceleration*dt;
	else
		return current += mAcceleration*dt;
}


void DynamicEntity::update(float dt)
{
	if (thor::length(mCurrentVelocity) > mMaxSpeed)
		thor::setLength(mCurrentVelocity, mMaxSpeed);
	mCurrentVelocity.x = accelerate(mCurrentVelocity.x, mGoalVelocity.x, dt);
	mCurrentVelocity.y = accelerate(mCurrentVelocity.y, mGoalVelocity.y, dt);
	setPosition(getPosition() + mCurrentVelocity * dt);
}

void DynamicEntity::setAcceleration(float accleration)
{
	mAcceleration = accleration;
}

float DynamicEntity::getRotation() const
{
	return mRotation;
}

void DynamicEntity::setRotation(float angle)
{
	mRotation = angle;
}
const sf::Vector2f & DynamicEntity::getVelocity()
{
	return mCurrentVelocity;
}
void DynamicEntity::setGoalVelocity(sf::Vector2f speed)
{
	mGoalVelocity = speed;
}
void DynamicEntity::setMaxSpeed(float speed)
{
	mMaxSpeed = speed;
}

void DynamicEntity::setRemotePosition(const sf::Vector2f & v)
{
	mRemotePosition = v;
}
const sf::Vector2f & DynamicEntity::getRemotePosition()
{
	return mRemotePosition;
}