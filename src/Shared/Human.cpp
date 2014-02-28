#include "Shared/Human.h"


Human::Human(Entity::ID id) : DynamicEntity(id)
{
	setType(Entity::Type::Human);
	setSize({ 50.f, 50.f });
	setAcceleration(1000.f);
	setMaxSpeed(300);
}


Human::~Human()
{
}
