#include "Client/Drawer.h"
#include "Shared/Wall.h"
#include "Shared/Human.h"
#include "Shared/Zombie.h"
#include "Shared/PickUp.h"

Drawer::Drawer(sf::RenderWindow & window, Textures & textures) :mWindow(window), mTextures(textures)
{

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
	mWindow.draw(wall);
}