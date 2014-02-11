#include "Shared\PickUp.h"


PickUp::PickUp(Entity::ID id) : StaticEntity(id)
{
	setType(Entity::Type::PickUp);
}


PickUp::~PickUp()
{
}
