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

namespace sf
{
	inline Packet& operator <<(Packet& packet, const Enemy& enemy)
	{
		sf::Vector2f pos, dir;
		pos = enemy.GetDirection();
		dir = enemy.GetPosition();
		return packet << pos << dir;
	}
	inline Packet& operator >>(Packet& packet, Enemy& enemy)
	{
		Vector2f position, direction;
		packet >> position >> direction;
		enemy.SetPosition(position);
		enemy.SetDirection(direction);
		return packet;
	}
}