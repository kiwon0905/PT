#include "Shared/Human.h"

const sf::Int32 Human::MaxHealth = 100;
const sf::Time Human::Skill1CoolDown = sf::seconds(0.2f);

Human::Human(Entity::ID id) : DynamicEntity(id)
{
	setType(Entity::Type::Human);
	setSize({ 50.f, 50.f });
	setAcceleration(3000.f);
	setMaxSpeed(100);
}


Human::~Human()
{
}

void Human::takeDamage(sf::Int32 amount)
{
	mHealth -= amount;
	if (mHealth < 0)
		kill();
}
