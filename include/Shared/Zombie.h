#pragma once
#include <SFML/System.hpp>
#include "Shared/DynamicEntity.h"



class Zombie : public DynamicEntity
{
public:
	static const sf::Int32 MaxHealth;
	static const sf::Time Skill1CoolDown;

	Zombie(Entity::ID id);
	~Zombie();
	
	bool skill1();

	void takeDamage(sf::Int32 amount);
	sf::FloatRect getMeeleAttackBox();

private:
	sf::Clock mSkill1LastUsed;
	sf::Int32 mHealth;
};