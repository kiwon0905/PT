#pragma once
#include <Thor/Particles.hpp>
#include <Thor/Input.hpp>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

class Zombie;
class PickUp;
class Human;
class Bullet;


class Drawer
{
public:
	Drawer(sf::RenderWindow & window, Textures & textures);

	void update(float dt);
	
	void drawZombie(const Zombie & z);
	void drawHuman(const Human & h);
	void drawPickUp(const PickUp & p);
	void drawBullet(const Bullet & b);

	thor::Connection addEmitter(thor::UniversalEmitter & emitter);
	thor::Connection addEmitter(thor::UniversalEmitter &, sf::Time);
private:
	Textures & mTextures;
	sf::RenderWindow & mWindow;
	thor::ParticleSystem mParticles;
};