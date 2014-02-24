#include "Shared/Zombie.h"


Zombie::Zombie(Entity::ID id) : DynamicEntity(id)
{
	setType(Entity::Type::Zombie);
	setSize({ 50.f, 50.f });
}


Zombie::~Zombie()
{
}
