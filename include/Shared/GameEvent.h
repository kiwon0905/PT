#pragma once

#include <SFML/System.hpp>
class Entity;
class DynamicEntity;
class GameWorld;

enum class GameEvent : sf::Int32
{
	MoveEntity,  //id, x, y
	RotateEntity, //id, r
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

struct RotateEntity : GameCommand
{
	void operator()() override;
	DynamicEntity * e;
	float a;
};