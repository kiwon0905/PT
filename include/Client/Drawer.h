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
	void setTextures(Textures & textures);
	void setTarget(sf::RenderTarget & target);

	void update(float dt);
	
	void drawWall(const Wall & w);
	void drawZombie(const Zombie & z);
	void drawHuman(const Human & h);
	void drawPickUp(const PickUp & p);
	void drawBullet(const Bullet & b);

	thor::Connection addEmitter(thor::UniversalEmitter & emitter);
	thor::Connection addEmitter(thor::UniversalEmitter &, sf::Time);
private:
	Textures * mTextures;
	sf::RenderTarget * mTarget;
	thor::ParticleSystem mParticles;
};