#include "Client/Drawer.h"
#include "Shared/Wall.h"
#include "Shared/Human.h"
#include "Shared/Zombie.h"
#include "Shared/PickUp.h"


void Drawer::update(float dt)
{
	mParticles.update(sf::seconds(dt));
}

void Drawer::drawWall(Textures & textures, sf::RenderWindow & window, const Wall & w)
{
	sf::RectangleShape wall;
	wall.setSize(w.getSize());
	wall.setPosition(w.getPosition());
	wall.setFillColor(sf::Color::Transparent);
	wall.setOutlineThickness(5.f);
	wall.setOutlineColor(sf::Color::Blue);
	window.draw(wall);
}


void Drawer::drawZombie(Textures & textures, sf::RenderWindow & window, const Zombie & z)
{
	sf::RectangleShape shape;
	shape.setOrigin(z.getSize() / 2.f);
	shape.setFillColor(sf::Color::Red);
	shape.setSize(z.getSize());

	shape.setPosition(z.getPosition() + z.getSize()/2.f);
	shape.setRotation(z.getRotation());

	window.draw(shape);

	sf::RectangleShape s;
	s.setPosition(z.getPosition());
	s.setSize(z.getSize());
	s.setFillColor(sf::Color::Transparent);
	s.setOutlineColor(sf::Color::Yellow);
	s.setOutlineThickness(5.f);
	window.draw(s);


}
void Drawer::drawHuman(Textures & textures, sf::RenderWindow & window, const Human & h)
{
	sf::RectangleShape shape;
	shape.setOrigin(h.getSize() / 2.f);
	shape.setFillColor(sf::Color::Black);
	shape.setSize(h.getSize());


	shape.setPosition(h.getPosition() + h.getSize()/2.f);
	shape.setRotation(h.getRotation());
	window.draw(shape);
}
void Drawer::drawPickUp(Textures & textures, sf::RenderWindow & window, const PickUp & p)
{

}
void Drawer::drawBullet(Textures & textures, sf::RenderWindow & window, const Bullet & b)
{

}