#include "Client/ClientGameWorld.h"
#include "Shared/ValueParser.h"
#include "Shared/Wall.h"
#include "Shared/GameEvent.h"
#include "Client/Application.h"
#include "Shared/NetProtocol.h"

GameWorld::GameWorld() 
{
	mEntitiesByType.resize(static_cast<std::size_t>(Entity::Type::Count));
}


GameWorld::~GameWorld()
{
}

Entity * GameWorld::getEntity(Entity::ID id)
{
	return mEntityMgr.get(id);
}

void GameWorld::addEntity(Entity::ID id)
{
	Entity * e = getEntity(id);
	mEntitiesByType.at(static_cast<std::size_t>(e->getType())).push_back(e);
}

void GameWorld::reset()
{
	mEntitiesByType.clear();
	mEntityMgr.destroyAll();
}

bool GameWorld::loadFromFile(const std::string & s)
{
	return mGameMap.loadFromFile(s);
}

std::vector<Entity * > & GameWorld::getEntitiesOfType(Entity::Type t)
{
	return mEntitiesByType.at(static_cast<std::size_t>(t));
}

void GameWorld::removeDeadEntities()
{
	//remove dead entities
	auto isDead = [this](Entity * e)
	{
		if (!e->isAlive())
		{
			mEntityMgr.destroy(e->getID());
			return true;
		}
		return false;
	};
	auto & zombies = getEntitiesOfType(Entity::Type::Zombie);
	zombies.erase(std::remove_if(zombies.begin(), zombies.end(), isDead), zombies.end());
	auto & humans = getEntitiesOfType(Entity::Type::Human);
	humans.erase(std::remove_if(humans.begin(), humans.end(), isDead), humans.end());
	auto & bullets = getEntitiesOfType(Entity::Type::Bullet);
	bullets.erase(std::remove_if(bullets.begin(), bullets.end(), isDead), bullets.end());
}

void GameWorld::step(float dt)
{
	removeDeadEntities();
	//std::cout <<"Bullet size: "<< getEntitiesOfType(Entity::Type::Bullet).size() << std::endl;
	for (auto & command : mCommands)
		(*command)();
	mCommands.clear();

	Entity * player =getEntity(mPlayerEntity);
	if (player)
	{
		player->update(dt);

		for (Entity * e : getEntitiesOfType(Entity::Type::Wall))
		{
			Wall * wall = static_cast<Wall*>(e);
			sf::FloatRect intersection;
			if (wall->getAABB().intersects(player->getAABB(), intersection))
			{
				if (intersection.width > intersection.height) //vertical
				{
					if (intersection.top < wall->getPosition().y + wall->getSize().y && player->getPosition().y < wall->getPosition().y)
						player->setPosition(player->getPosition() + sf::Vector2f(0, -intersection.height));

					else if (intersection.top + intersection.height > wall->getPosition().y)
						player->setPosition(player->getPosition() + sf::Vector2f(0, intersection.height));
				}
				else
				{
					if (intersection.left < wall->getPosition().x + wall->getSize().x && player->getPosition().x < wall->getPosition().x)
						player->setPosition(player->getPosition() + sf::Vector2f(-intersection.width, 0));

					else if (intersection.left + intersection.width > wall->getPosition().x)
						player->setPosition(player->getPosition() + sf::Vector2f(intersection.width, 0));
				}
			}
		}

	
	
	}
		

	mDrawer.update(dt);
}

void GameWorld::draw(Textures & textures, sf::RenderWindow & window)
{
	sf::Texture & texture = *textures.get("assets/background.png");
	texture.setRepeated(true);
	texture.setSmooth(true);

	sf::Sprite background;
	background.setTexture(texture);
	background.setTextureRect(sf::IntRect(0, 0, mGameMap.getSize().x, mGameMap.getSize().y));
	window.draw(background);

	for (Entity * e : getEntitiesOfType(Entity::Type::Wall))
		mDrawer.drawWall(textures, window, static_cast<Wall &>(*e));
	for (Entity * e : getEntitiesOfType(Entity::Type::PickUp))
		mDrawer.drawPickUp(textures, window, static_cast<PickUp &>(*e));
	for (Entity * e : getEntitiesOfType(Entity::Type::Human))
		mDrawer.drawHuman(textures, window, static_cast<Human &>(*e));
	for (Entity * e : getEntitiesOfType(Entity::Type::Zombie))
		mDrawer.drawZombie(textures, window, static_cast<Zombie &>(*e));
	for (Entity * e : getEntitiesOfType(Entity::Type::Bullet))
		mDrawer.drawBullet(textures, window, static_cast<Bullet &>(*e));
	
	Entity * player = getEntity(mPlayerEntity);
	if (player)
	{
		sf::View view = window.getView();
		view.setCenter(player->getCenter());
		window.setView(view);
		if (player->getType() == Entity::Type::Human)
			mDrawer.drawHuman(textures, window, static_cast<Human &>(*player));
		else if (player->getType() == Entity::Type::Zombie)
			mDrawer.drawZombie(textures, window, static_cast<Zombie &>(*player));
	}

}

void GameWorld::setPlayerEntity(Entity::ID id)
{
	mPlayerEntity = id;
}

void GameWorld::handlePacket(sf::Packet & packet)
{
	GameEvent ev;
	packet >> ev;
	switch (ev)
	{
	case GameEvent::MoveEntity:
	{
		sf::Int32 num;
		packet >> num;
		for (sf::Int32 i = 0; i < num; ++i)
		{
			Entity::ID id;
			packet >> id;
			Entity * e = getEntity(id);

			float x, y;
			packet >> x >> y;
			
			if (id != mPlayerEntity && e)
			{
				MoveEntity * move = new MoveEntity;
				move->e = e;
				move->x = x;
				move->y = y;
				mCommands.emplace_back(move);
			}
		}		
	}
		break;

	case GameEvent::RotateEntity:
	{
		sf::Int32 num;
		packet >> num;
		for (sf::Int32 i = 0; i < num; ++i)
		{
			Entity::ID id;
			packet >> id;
			Entity * e = getEntity(id);

			float a;
			packet >> a;

			if (id != mPlayerEntity && e)
			{
				RotateEntity * rotate = new RotateEntity;
				rotate->e = static_cast<DynamicEntity *>(e);
				rotate->a = a;
				mCommands.emplace_back(rotate);
			}
		}
	}
		break;
	case GameEvent::DestroyEntity:
	{
		sf::Int32 num;
		packet >> num;
		for (sf::Int32 i = 0; i < num; ++i)
		{
			Entity::ID id;
			packet >> id;
			if (getEntity(id)->getType() == Entity::Type::Zombie)
				std::cout << "Destroying zombie\n";
			getEntity(id)->kill();
		}

	}
		break;

	case GameEvent::ShootBullet:
	{
		Entity::ID id;
		float direction, x, y;
		packet >> id >> direction >> x >> y;
		Bullet * b = createEntity<Bullet>(id, Entity::Type::Bullet);
		b->setPosition({ x, y });
		b->setDirection(direction);
		addEntity(b->getID());

	}
		break;
	default:
		break;
	}

}