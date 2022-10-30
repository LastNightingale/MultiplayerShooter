#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include "GameMath.hpp"
#define Enemy_Size 50
#define Enemy_Color Color::Red

using namespace sf;

class Enemy : public Entity
{
private:
	Vector2f m_Direction;
public:
	Enemy(Vector2f position, Vector2f direction);
	Enemy();
	void Update(float dt) override;
	bool Collided(Entity* other) override;
	Vector2f GetDirection() const;
	void SetDirection(Vector2f direction);
};
