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
	shape.setFillColor(sf::Color::Red);
	shape.setSize(z.getSize());
	shape.setOrigin(z.getSize() / 2.f);


	shape.setPosition(z.getPosition()+z.getSize()/2.f);

	shape.setOrigin(z.getSize() / 2.f);
	shape.setRotation(z.getRotation());

	mTarget->draw(shape);
}
void Drawer::drawHuman(const Human & h)
{
	sf::RectangleShape shape;
	shape.setFillColor(sf::Color::Black);
	shape.setSize(h.getSize());
	shape.setOrigin(h.getSize() / 2.f);

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

void Drawer::drawTexture(const std::string & s)
{
	sf::Sprite sprite;
	sprite.setTextureRect(sf::IntRect(0, 0, 800, 600));
	sprite.setTexture(*mTextures->get(s));
	mTarget->draw(sprite);
}