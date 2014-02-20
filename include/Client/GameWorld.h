#pragma once
#include <vector>

#include "Shared/Entity.h"
#include "Shared/EntityManager.h"
#include "Client/Drawer.h"

class Application;
//Client
class GameWorld
{
public:
	GameWorld();
	~GameWorld();
	
	void initialize(Textures & textures, sf::RenderTarget & target);

	Entity * createEntity(Entity::ID id, Entity::Type type);
	Entity * getEntity(Entity::ID id);
	void addEntity(Entity::ID id);
	
	bool loadFromFile(const std::string & s);
	void reset();

	void step(float dt);
	void draw();

private:
	const std::vector<Entity * > & getEntitiesOfType(Entity::Type t) const;
	
	Drawer mDrawer;
	EntityManager mEntityMgr;
	std::vector<std::vector<Entity *>> mEntitiesByType;
	

};

