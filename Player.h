#pragma once
#include "Entity.h"
class Bullet;

class Player : public Entity
{
public:
	Player();
	void Update(float dt) override;
	bool Collided(Entity* other) override;
	Bullet Shoot(sf::Vector2i MousePosition);
};

