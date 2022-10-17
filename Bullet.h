#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "GameMath.hpp"
#define Bullet_Size 30
#define Bullet_Color Color::White

using namespace sf;

class Bullet : public Entity
{
private:
	//Player* m_Owner;
	Vector2f m_Direction;
public:
	Bullet(Player* owner, Vector2f direction);
	Bullet(Bullet&& other);
	Bullet();
	Bullet(const Bullet&) = default;
	void Update(float dt) override;
	bool Collided(Entity* other) override;
	//Player* GetOWner();
	Vector2f GetDirection() const;
	void SetDirection(Vector2f direction);
};

namespace sf
{
	inline Packet& operator <<(Packet& packet, const Bullet& bullet)
	{
		sf::Vector2f pos, dir;
		pos = bullet.GetDirection();
		dir = bullet.GetPosition();
		return packet << pos << dir;
	}
	inline Packet& operator >>(Packet& packet, Bullet& bullet)
	{
		Vector2f position, direction;
		packet >> position >> direction;
		bullet.SetPosition(position);
		bullet.SetDirection(direction);
		return packet;
	}	
}