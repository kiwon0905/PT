#pragma once

#include "Shared/Entity.h"

class DynamicEntity : public Entity
{
public:
	DynamicEntity(Entity::ID id);
	virtual ~DynamicEntity()=default;
	
	void update(float dt) override;

	float getRotation() const;
	void setRotation(float angle);
	const sf::Vector2f & getVelocity();
	void setGoalVelocity(sf::Vector2f v);
	void setAcceleration(float accleration);

	void setMaxSpeed(float vel);

	void setRemotePosition(const sf::Vector2f & v);
	const sf::Vector2f & getRemotePosition();
private:
	float accelerate(float current, float goal, float dt);
	
	sf::Vector2f mGoalVelocity;
	sf::Vector2f mCurrentVelocity;
	float mAcceleration;
	float mRotation;
	float mMaxSpeed;

	sf::Vector2f mRemotePosition;
};

