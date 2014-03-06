#pragma once
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <memory>

class Entity
{
public:
	typedef sf::Uint32 ID;
	enum class Type : sf::Int32
	{
		Human,
		Zombie,
		PickUp,
		Wall,
		Count
	};

	Entity(ID id);
	virtual ~Entity()=default;

	Entity::ID getID();
	void move(const sf::Vector2f & v);
	void setPosition(const sf::Vector2f & v);
	const sf::Vector2f & getPosition() const;
	const sf::Vector2f & getCenter() const;
	const sf::Vector2f & getSize() const;
	void setSize(const sf::Vector2f & v);
	Type getType() const;
	void setType(Type type);
	sf::FloatRect getAABB() const ;
	bool isAlive() const;
	void kill();

	virtual void update(float dt)=0;
	
private:
	ID mID;
	Type mType;
	sf::Vector2f mPosition;
	sf::Vector2f mSize;
	bool mAlive;

};
