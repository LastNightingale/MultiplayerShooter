#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "GameMath.hpp"

using namespace sf;

class Bullet : public Entity
{
private:
	Player* m_Owner;
	Vector2f m_Direction;
public:
	Bullet(Player* owner, Vector2f direction);
	Bullet(Bullet&& other);
	void Update(float dt) override;
	bool Collided(Entity* other) override;
	Player* GetOWner();
};