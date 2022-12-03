#pragma once
#include "Entity.h"
#include <unordered_map>
#include "Keys.hpp"
class Bullet;
#define Player_Size 75
#define Player_Color Color::Blue

class Player : public Entity
{
	
public:
	Vector2f m_Direction;
	bool isReady = true;
	Player();
	void Update(float dt) override;
	bool Collided(Entity* other) override;
	Bullet Shoot(sf::Vector2i MousePosition);
	std::unordered_map<sf::Keyboard::Key, bool> m_Keys = {
		{ sf::Keyboard::Key::D, false },
		{ sf::Keyboard::Key::A, false } ,
		{ sf::Keyboard::Key::W, false } ,
		{ sf::Keyboard::Key::S, false }
	};
};
//
//namespace sf
//{
//	inline Packet& operator <<(Packet& packet, const Player& player)
//	{		
//		sf::Vector2f pos = player.GetPosition();
//		return packet << pos;
//	}
//	inline Packet& operator >>(Packet& packet, Player& player)
//	{
//		Vector2f position;
//		packet >> position;
//		player.SetPosition(position);
//		return packet; 
//	}
//}

