#include "Shared/Human.h"


Human::Human(Entity::ID id) : DynamicEntity(id)
{
	setType(Entity::Type::Human);
}


Human::~Human()
{
}
