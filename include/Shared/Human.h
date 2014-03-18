#pragma once
#include "Shared/DynamicEntity.h"
#include <SFML/System.hpp>

class Human : public DynamicEntity
{
public:
	Human(Entity::ID id);
	~Human();

	static const sf::Int32 MaxHealth;
	static const sf::Time Skill1CoolDown;	

	void takeDamage(sf::Int32 amount);
private:
	sf::Int32 mHealth;
};

