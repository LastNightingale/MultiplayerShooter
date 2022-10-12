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
	Packet& operator <<(Packet& packet, const Enemy& enemy)
	{
		return packet << enemy.GetPosition() << enemy.GetDirection();
	}
	Packet& operator >>(Packet& packet, Enemy& enemy)
	{
		Vector2f position, direction;
		packet >> position >> direction;
		enemy.SetPosition(position);
		enemy.SetDirection(direction);
		return packet;
	}

	Packet& operator <<(Packet& packet, const std::vector<Enemy>& enemies)
	{
		packet << enemies.size();
		for (size_t i = 0; i < enemies.size(); ++i)
			packet << enemies[i];
		return packet;
	}
	Packet& operator >>(Packet& packet, std::vector<Enemy>& enemies)
	{
		int size;
		Enemy enemy;
		packet >> size;
		for (int i = 0; i < size; ++i)
		{
			packet >> enemy;
			enemies.push_back(Enemy(enemy));
		}
		return packet;
	}
}