#include "Shared/Wall.h"


Wall::Wall(Entity::ID id) : StaticEntity(id)
{
	setType(Entity::Type::Wall);
}


Wall::~Wall()
{
}
