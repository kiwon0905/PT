#include "Shared/Entity.h"
#include <SFML\Graphics.hpp>


Entity::Entity(Entity::ID id) : mID(id), mPosition(), mSize(), mAlive(true)
{
}

Entity::ID Entity::getID()
{
	return  mID;
}

void Entity::setPosition(const sf::Vector2f & v)
{
	mPosition = v;
}

const sf::Vector2f & Entity::getPosition() const
{
	return mPosition;
}

const sf::Vector2f & Entity::getCenter() const
{
	return mPosition + getSize() / 2.f;
}
const sf::Vector2f & Entity::getSize() const
{
	return mSize;
}
void Entity::setSize(const sf::Vector2f & v)
{
	mSize = v;
}

Entity::Type Entity::getType() const
{
	return mType;
}

void Entity::setType(Type type)
{
	mType = type;
}

sf::FloatRect Entity::getAABB() const
{
	return sf::FloatRect(mPosition.x - mSize.x / 2, mPosition.y - mSize.y / 2, mSize.x, mPosition.y);
}
bool Entity::isAlive() const
{
	return mAlive;
}

void Entity::kill()
{
	mAlive = false;
}
