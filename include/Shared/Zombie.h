#pragma once
#include <SFML/System.hpp>
#include "Shared/DynamicEntity.h"

class Zombie : public DynamicEntity
{
public:
	static const sf::Int32 MaxHealth;

	Zombie(Entity::ID id);
	~Zombie();

	void takeDamage(sf::Int32 amount);
private:
	sf::Int32 mHealth;
	
};

