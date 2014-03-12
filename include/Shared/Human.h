#pragma once
#include "Shared/DynamicEntity.h"
#include <SFML/System.hpp>

class Human : public DynamicEntity
{
public:
	Human(Entity::ID id);
	~Human();

	static const sf::Time Skill1CoolDown;
};

