#pragma once
#include "Shared\Entity.h"

class StaticEntity : public Entity
{
public:
	StaticEntity(Entity::ID id);
	virtual ~StaticEntity() = default;

	void update(float dt) override;
};