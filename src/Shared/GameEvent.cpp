#include "Shared/GameEvent.h"
#include "Shared/Entity.h"
#include "Shared/DynamicEntity.h"

void MoveEntity::operator()()
{
	e->setPosition({ x, y });
}

void RotateEntity::operator()()
{
	e->setRotation(a);
}