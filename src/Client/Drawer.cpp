#include "Client/Drawer.h"
#include "Shared/Wall.h"
#include "Shared/Human.h"
#include "Shared/Zombie.h"
#include "Shared/PickUp.h"
#include "Shared/Bullet.h"


void Drawer::update(float dt)
{
	mParticles.update(sf::seconds(dt));
}

void Drawer::drawWall(Textures & textures, sf::RenderWindow & window, const Wall & w)
{
	sf::RectangleShape wall;
	wall.setSize(w.getSize());
	wall.setPosition(w.getPosition());
	wall.setFillColor(sf::Color::Blue);


	window.draw(wall);
}


void Drawer::drawZombie(Textures & textures, sf::RenderWindow & window, const Zombie & z)
{
	std::shared_ptr<sf::Texture> human = textures.get("assets/zombie.png");
	sf::Sprite sprite;
	sprite.setTexture(*human);
	sf::Vector2f origin = z.getSize() / 2.f;
	origin.y += 10;
	sprite.setOrigin(origin);
	sprite.setPosition(z.getPosition() + z.getSize() / 2.f);
	sprite.setRotation(z.getRotation() + 90);
	window.draw(sprite);


	if (z.isSkill1Active())
	{
		std::shared_ptr<sf::Texture> blood = textures.get("assets/attack blood.png");
		sf::Sprite attackblood;
		attackblood.setTexture(*blood);	sf::FloatRect aabb = z.getMeeleAttackBox();
	
		attackblood.setOrigin(aabb.width / 2.f, 100);
		attackblood.setPosition(z.getCenter());
		attackblood.setRotation(z.getRotation() + 90);

		window.draw(attackblood);
	}


	

}
void Drawer::drawHuman(Textures & textures, sf::RenderWindow & window, const Human & h)
{
	std::shared_ptr<sf::Texture> human = textures.get("assets/player_human.png");
	sf::Sprite sprite;
	sprite.setTexture(*human);

	

	sf::Vector2f origin = h.getSize() / 2.f;
	origin.y += 10;
	sprite.setOrigin(origin);
	sprite.setPosition(h.getPosition() + h.getSize() / 2.f);
	sprite.setRotation(h.getRotation() + 90);
	window.draw(sprite);
}
void Drawer::drawPickUp(Textures & textures, sf::RenderWindow & window, const PickUp & p)
{

}
void Drawer::drawBullet(Textures & textures, sf::RenderWindow & window, const Bullet & b)
{
	sf::CircleShape shape;
	
	shape.setFillColor(sf::Color::Yellow);
	shape.setRadius(b.getSize().x/2);


	shape.setPosition(b.getPosition());

	window.draw(shape);
}