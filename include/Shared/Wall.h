#pragma once
#include "Shared/StaticEntity.h"


class Wall : public StaticEntity
{
public:
	Wall(Entity::ID id);
	~Wall();
};

