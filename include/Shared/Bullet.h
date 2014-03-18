#pragma once
#include "Shared/DynamicEntity.h"


class Bullet : public DynamicEntity
{
public:
	Bullet(Entity::ID id);
	~Bullet();	
	
	void setDirection(float angle);
	//const static
};

