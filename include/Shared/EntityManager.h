#pragma once
#include <unordered_map>
#include "Shared/Human.h"
#include "Shared/Zombie.h"
#include "Shared/PickUp.h"
#include "Shared/Wall.h"
#include "Shared/Bullet.h"

class EntityManager
{
public:
	EntityManager();
	~EntityManager();

	template <class T>
	T * create(Entity::ID id, Entity::Type type);
	void destroy(Entity::ID id);
	void destroyAll();
	Entity * get(Entity::ID id);
private:
	std::unordered_map<Entity::ID, std::unique_ptr<Entity>> mEntities;
};

template <class T>
T * EntityManager::create(Entity::ID id, Entity::Type type)
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
	case Entity::Type::Wall:
		e.reset(new Wall(id));
		break;
	case Entity::Type::Bullet:
		e.reset(new Bullet(id));
	default:
		break;
	}
	mEntities.insert({ id, std::move(e) });
	return static_cast<T*>(mEntities.at(id).get());
}
