#pragma once
#include "Shared/DynamicEntity.h"


class Bullet : public DynamicEntity
{
public:
	const static sf::Int32 Damage;

	Bullet(Entity::ID id);
	~Bullet();	
	
	void setDirection(float angle);
	//const static
};

