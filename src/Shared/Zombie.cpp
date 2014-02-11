#include "Shared/Zombie.h"


Zombie::Zombie(Entity::ID id) : DynamicEntity(id)
{
	setType(Entity::Type::Zombie);
}


Zombie::~Zombie()
{
}
