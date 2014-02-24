#pragma once
#include "Shared/Entity.h"

class Player
{
public:
	Player();
	~Player();
	enum Action
	{
		MoveE,	//0
		MoveN,	//90
		MoveW,	//180
		MoveS,	//270
		Skill1
	};
private:
	Entity::ID mID;
};

