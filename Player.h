#pragma once
#include "Entity.h"
class Bullet;
#define Player_Size 75
#define Player_Color Color::Blue

class Player : public Entity
{
public:
	Player();
	void Update(float dt) override;
	bool Collided(Entity* other) override;
	Bullet Shoot(sf::Vector2i MousePosition);
};

namespace sf
{
	inline Packet& operator <<(Packet& packet, const Player& player)
	{		
		sf::Vector2f pos = player.GetPosition();
		return packet << pos;
	}
	inline Packet& operator >>(Packet& packet, Player& player)
	{
		Vector2f position;
		packet >> position;
		player.SetPosition(position);
		return packet; 
	}
}

