#include "Shared\Entity.h"

class StaticEntity : public Entity
{
public:
	StaticEntity(Entity::ID id);
	~StaticEntity() = 0;

	void update(float dt) override;
};