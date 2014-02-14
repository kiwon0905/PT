#include "Shared/GameEvent.h"
#include "Shared/Entity.h"

MoveEntity::MoveEntity(Entity * e, float x, float y) : e(e), x(x), y(y)
{

}
void MoveEntity::operator()()
{
	e->setPosition({ x, y });
}