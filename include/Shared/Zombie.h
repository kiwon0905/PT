#pragma once
#include "Shared/DynamicEntity.h"

class Zombie : public DynamicEntity
{
public:
	Zombie(Entity::ID id);
	~Zombie();
};

