#include "Shared/GameEvent.h"
#include "Shared/Entity.h"

void MoveEntity::operator()()
{
	e->setPosition({ x, y });
}