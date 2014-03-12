#pragma once
#include "Shared/DynamicEntity.h"


class Bullet : public DynamicEntity
{
public:
	Bullet(Entity::ID id);
	void setDirection(float angle);
	~Bullet();
};

