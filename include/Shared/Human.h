#pragma once
#include "Shared/DynamicEntity.h"

class Human : public DynamicEntity
{
public:
	Human(Entity::ID id);
	~Human();
};

