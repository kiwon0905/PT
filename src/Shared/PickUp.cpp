#include "Shared\PickUp.h"


PickUp::PickUp(Entity::ID id) : StaticEntity(id)
{
	setType(Entity::Type::PickUp);
	setSize({ 50.f, 50.f });
}


PickUp::~PickUp()
{
}
