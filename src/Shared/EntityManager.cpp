#include "Shared/EntityManager.h"
#include "Shared/Human.h"
#include "Shared/Zombie.h"
#include "Shared/PickUp.h"

EntityManager::EntityManager()
{
}


EntityManager::~EntityManager()
{
}

Entity * EntityManager::create(Entity::ID id, Entity::Type type)
{
	std::unique_ptr<Entity> e;

	switch (type)
	{
	case Entity::Type::Human:
		e.reset(new Human(id));
		break;
	case Entity::Type::Zombie:
		e.reset(new Zombie(id));
		break;
	case Entity::Type::PickUp:
		e.reset(new PickUp(id));
		break;
	default:
		break;
	}
	mEntities.insert({ id, std::move(e) });

}
void EntityManager::destroy(Entity::ID id)
{
	mEntities.erase(id);
}
Entity * EntityManager::get(Entity::ID id)
{
	return mEntities.at(id).get();
}