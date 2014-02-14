#pragma once

#include <SFML/System.hpp>
class Entity;
class GameWorld;

enum class GameEvent : sf::Int32
{
	MoveEntity,

	CreateHuman,
	CreateWall,
	CreatePickUp,
	
	DestroyHuman,
	DestroyPickUp,
	
};

struct GameCommand
{
	virtual void operator()()=0;
};

struct MoveEntity : GameCommand
{
	MoveEntity(Entity * e, float x, float y);
	void operator()() override;
	Entity * e;
	float x, y;
};