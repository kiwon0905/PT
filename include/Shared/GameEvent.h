#pragma once

#include <SFML/System.hpp>
class Entity;
class GameWorld;

enum class GameEvent : sf::Int32
{
	MoveEntity,  //id, x, y
	
	DestroyEntity
};

struct GameCommand
{
	virtual void operator()()=0;
};

struct MoveEntity : GameCommand
{
	void operator()() override;
	Entity * e;
	float x, y;
};