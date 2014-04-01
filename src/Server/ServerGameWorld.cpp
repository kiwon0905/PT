#include "Server/ServerGameWorld.h"
#include "Server/ServerGame.h"
#include "Shared/ValueParser.h"
#include "Shared/Wall.h"
#include "Shared/NetProtocol.h"
#include "Shared/GameEvent.h"
#include "Shared/Bullet.h"

#include <Thor/Vectors.hpp>
#include <Thor/Math.hpp>
#include <iostream>

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

std::vector<Entity *> & GameWorld::getEntitiesOfType(Entity::Type t)
{
	return mEntitiesByType.at(static_cast<std::size_t>(t));
}

void GameWorld::loadMap(const std::string & s)
{
	mGameMap.loadFromFile(s);

	for (auto & wall : mGameMap.getWalls())
	{
		Wall * w = createEntity<Wall>(Entity::Type::Wall);
		w->setPosition({ wall.left, wall.top });
		w->setSize({ wall.width, wall.height });
		addEntity(w->getID());
	}
	//std::cout << "width: " << mGameMap.getSize().x << "height: " << mGameMap.getSize().y << std::endl;
}

void GameWorld::leave(Peer * p, Game & game)
{
	//if this peer has an entity associated with it, kill it
	if (mEntitiesByPeer.count(p) == 1)
		getEntity(mEntitiesByPeer[p])->kill();
	
	sf::Packet * packet = new sf::Packet;
	*packet << Sv::GameEvent << GameEvent::DestroyEntity << sf::Int32(1) << mEntitiesByPeer[p];
	game.pushPacket(p, packet, true);

	mEntitiesByPeer.erase(p);
}

void GameWorld::removeDeadEntities(Game & g)
{
	//remove dead entities
	std::vector<Entity::ID> deadEntities;
	auto isDead = [this, &deadEntities](Entity * e)
	{
		if (!e->isAlive())
		{
			deadEntities.push_back(e->getID());
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

	if (deadEntities.size() > 0)
	{
		sf::Packet * packet = new sf::Packet;
		*packet << Sv::GameEvent << GameEvent::DestroyEntity << sf::Int32(deadEntities.size());
		for (Entity::ID id : deadEntities)
			*packet << id;
		g.pushPacket(nullptr, packet, true);
	}
	deadEntities.clear();

}

void GameWorld::step(Game & game, float dt)
{
	removeDeadEntities(game);
	for (auto & command : mCommands)
		(*command)();
	mCommands.clear();


	for (Entity * e : getEntitiesOfType(Entity::Type::Bullet))
	{
		static_cast<Bullet*>(e)->update(dt);
	}

	//Bullet collision against wall
	for (Entity * e : getEntitiesOfType(Entity::Type::Bullet))
	{
		bool collided = !mGameMap.getBound().intersects(e->getAABB());
		for (Entity * wall : getEntitiesOfType(Entity::Type::Wall))
		{
			if (wall->getAABB().intersects(e->getAABB()))
			{
				collided = true;
				break;
			}

		}
		if (collided)
			e->kill();

	}

	for (Entity * e : getEntitiesOfType(Entity::Type::Bullet))
	{

		for (Entity * zombie : getEntitiesOfType(Entity::Type::Zombie))
		{

			if (zombie->getAABB().intersects(e->getAABB()))
			{
				static_cast<Zombie*>(zombie)->takeDamage(Bullet::Damage);
				break;
				
			}

		}

	}
}

void GameWorld::sync(Game & game)
{
	
	//position
	sf::Packet * packet = new sf::Packet;
	*packet << Sv::GameEvent << GameEvent::MoveEntity << sf::Int32(getEntitiesOfType(Entity::Type::Zombie).size());
	for (Entity * e : getEntitiesOfType(Entity::Type::Zombie))
		*packet << e->getID() <<e->getPosition().x << e->getPosition().y;
	game.pushPacket(nullptr, packet, true);
	
	sf::Packet * packet2 = new sf::Packet;
	*packet2 << Sv::GameEvent << GameEvent::MoveEntity << sf::Int32(getEntitiesOfType(Entity::Type::Human).size());
	for (Entity * e : getEntitiesOfType(Entity::Type::Human))
		*packet2 << e->getID() << e->getPosition().x << e->getPosition().y;
	game.pushPacket(nullptr, packet2, true);

	sf::Packet * packet3 = new sf::Packet;
	*packet3 << Sv::GameEvent << GameEvent::RotateEntity << sf::Int32(getEntitiesOfType(Entity::Type::Zombie).size());
	for (Entity * e : getEntitiesOfType(Entity::Type::Zombie))
		*packet3 << e->getID() << static_cast<DynamicEntity*>(e)->getRotation();
	game.pushPacket(nullptr, packet3, true);

	sf::Packet * packet4 = new sf::Packet;
	*packet4 << Sv::GameEvent << GameEvent::RotateEntity << sf::Int32(getEntitiesOfType(Entity::Type::Human).size());
	for (Entity * e : getEntitiesOfType(Entity::Type::Human))
		*packet4 << e->getID() << static_cast<DynamicEntity*>(e)->getRotation();
	game.pushPacket(nullptr, packet4, true);
	
	sf::Packet * packet5 = new sf::Packet;
	*packet5 << Sv::GameEvent << GameEvent::MoveEntity << sf::Int32(getEntitiesOfType(Entity::Type::Bullet).size());
	for (Entity * e : getEntitiesOfType(Entity::Type::Bullet))
		*packet5 << e->getID() << e->getPosition().x << e->getPosition().y;
	game.pushPacket(nullptr, packet5, true);
}


void GameWorld::handlePacket(Game & g, sf::Packet & packet)
{
	GameEvent ev;
	packet >> ev;
	switch (ev)
	{
	case GameEvent::MoveEntity:
	{
		Entity::ID id;
		packet >> id;
		Entity * e = getEntity(id);
		if (e)
		{
			float x, y;
			packet >> x >> y;
			MoveEntity * move = new MoveEntity;
			move->e = e;
			move->x = x;
			move->y = y;
			mCommands.emplace_back(move);
		}
		break;
	}
	case GameEvent::RotateEntity:
	{
		Entity::ID id;
		packet >> id;
		Entity * e = getEntity(id);
		if (e)
		{
			float angle;
			packet >> angle;
			RotateEntity * rotate = new RotateEntity;
			rotate->e = static_cast<DynamicEntity *>(e);
			rotate->a = angle;
			mCommands.emplace_back(rotate);
		}
	}
	break;
	case GameEvent::ShootBullet:
	{
		Entity::ID id;


		float direction, x, y;
		packet >> id >> direction >> x>> y;
		
		Human * entity = static_cast<Human*>(getEntity(id));
		

		Bullet * bullet = createEntity<Bullet>(Entity::Type::Bullet);
		bullet->setPosition({ x, y });
		bullet->setDirection(thor::randomDev(direction, thor::length(entity->getVelocity())/100 + 1));


		addEntity(bullet->getID());
		
		sf::Packet * packet = new sf::Packet;
		*packet << Sv::GameEvent << GameEvent::ShootBullet << bullet->getID() << direction << x << y;
		g.pushPacket(nullptr, packet, true);
	}
		break;
	case GameEvent::DestroyEntity:
		break;
	default:
		break;
	}
}