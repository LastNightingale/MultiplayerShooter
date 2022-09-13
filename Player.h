#pragma once
#include "Entity.h"
class Player : public Entity
{
public:
	Player();
	void Update(float dt) override;
	bool Collided(Entity* other) override;
};

