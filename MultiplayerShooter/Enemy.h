#pragma once
#include "Entity.h"
#include <SFML/Graphics.hpp>
#include "GameMath.hpp"

using namespace sf;

class Enemy : public Entity
{
private:
	Vector2f m_Direction;
public:
	Enemy(Vector2f position, Vector2f direction);
	void Update(float dt) override;
};

