#include "Shared/EntityManager.h"


EntityManager::EntityManager()
{
}


EntityManager::~EntityManager()
{
}

void EntityManager::destroy(Entity::ID id)
{
	mEntities.erase(id);
}
void EntityManager::destroyAll()
{
	mEntities.clear();
}
Entity * EntityManager::get(Entity::ID id)
{
	if (mEntities.count(id) != 0)
		return mEntities.at(id).get();
	else
		return nullptr;
}