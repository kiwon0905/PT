#pragma once
#include <Thor/Particles.hpp>
#include <Thor/Input.hpp>
#include <SFML/Graphics.hpp>
#include "ResourceManager.h"

class Zombie;
class PickUp;
class Human;
class Bullet;
class Wall;

class Drawer
{
public:
	void update(float dt);
	
	void drawWall(Textures & textures, sf::RenderWindow & window, const Wall & w);
	void drawZombie(Textures & textures, sf::RenderWindow & window, const Zombie & z);
	void drawHuman(Textures & textures, sf::RenderWindow & window, const Human & h);
	void drawPickUp(Textures & textures, sf::RenderWindow & window, const PickUp & p);
	void drawBullet(Textures & textures, sf::RenderWindow & window, const Bullet & b);


	thor::Connection addEmitter(thor::UniversalEmitter & emitter);
	thor::Connection addEmitter(thor::UniversalEmitter &, sf::Time);
private:
	thor::ParticleSystem mParticles;
};