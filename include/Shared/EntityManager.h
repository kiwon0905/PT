#pragma once
#include <unordered_map>
#include "Shared/Entity.h"

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	Entity * create(Entity::ID id, Entity::Type type);
	void destroy(Entity::ID id);
	Entity * get(Entity::ID id);
private:
	std::unordered_map<Entity::ID, std::unique_ptr<Entity>> mEntities;
};

