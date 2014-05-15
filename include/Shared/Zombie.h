#pragma once
#include <SFML/System.hpp>
#include "Shared/DynamicEntity.h"



class Zombie : public DynamicEntity
{
public:
	static const sf::Int32 MaxHealth;
	static const sf::Int32 Damage;
	static const sf::Time Skill1CoolDown;
	static const sf::Time Skill1Duration;


	Zombie(Entity::ID id);
	~Zombie();
	
	bool skill1();
	bool isSkill1Active() const;

	void takeDamage(sf::Int32 amount);
	sf::FloatRect getMeeleAttackBox() const;

private:
	sf::Clock mSkill1LastUsed;
	sf::Int32 mHealth;
};