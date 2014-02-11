#include "Client\Drawer.h"

Drawer::Drawer(sf::RenderWindow & window, Textures & textures) :mWindow(window), mTextures(textures)
{

}

void Drawer::update(float dt)
{
	mParticles.update(sf::seconds(dt));
}