#include "Client/Drawer.h"
#include "Shared/Wall.h"
#include "Shared/Human.h"
#include "Shared/Zombie.h"
#include "Shared/PickUp.h"

void Drawer::setTextures(Textures & textures)
{
	mTextures = &textures;
}

void Drawer::setTarget(sf::RenderTarget & target)
{
	mTarget = &target;
}
void Drawer::update(float dt)
{
	mParticles.update(sf::seconds(dt));
}

void Drawer::drawWall(const Wall & w)
{
	sf::RectangleShape wall;
	wall.setSize(w.getSize());
	wall.setPosition(w.getPosition());
	wall.setFillColor(sf::Color::Transparent);
	wall.setOutlineThickness(5.f);
	wall.setOutlineColor(sf::Color::Blue);
	mTarget->draw(wall);
}


void Drawer::drawZombie(const Zombie & z)
{
	sf::RectangleShape shape;
	shape.setOrigin(z.getSize() / 2.f);
	shape.setFillColor(sf::Color::Red);
	shape.setSize(z.getSize());

	shape.setPosition(z.getPosition() + z.getSize()/2.f);
	shape.setRotation(z.getRotation());

	mTarget->draw(shape);

	sf::RectangleShape s;
	s.setFillColor(sf::Color::Transparent);
	s.setOutlineThickness(5.f);
	s.setOutlineColor(sf::Color::Yellow);
	sf::FloatRect bound = z.getAABB();
	s.setSize({ bound.width, bound.height });
	s.setPosition(bound.left, bound.top);

	mTarget->draw(s);
}
void Drawer::drawHuman(const Human & h)
{
	sf::RectangleShape shape;
	shape.setOrigin(h.getSize() / 2.f);
	shape.setFillColor(sf::Color::Black);
	shape.setSize(h.getSize());


	shape.setPosition(h.getPosition() + h.getSize()/2.f);
	shape.setRotation(h.getRotation());
	mTarget->draw(shape);
}
void Drawer::drawPickUp(const PickUp & p)
{

}
void Drawer::drawBullet(const Bullet & b)
{

}