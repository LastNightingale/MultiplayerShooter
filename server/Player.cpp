#include "Player.h"
#include "Bullet.h"
#include "GameMath.hpp"
#include <iostream>


Player::Player()
{
	m_Body.setFillColor(Player_Color);
	m_Body.setSize(Vector2f(Player_Size, Player_Size));
	m_Body.setOrigin(Vector2f(m_Body.getSize().x / 2, m_Body.getSize().y / 2));
	m_Body.setPosition(Vector2f(400, 400));
}

void Player::Update(float dt) 
{
	//Move(m_Direction * dt * 10.f);
	//m_Direction = { 0, 0 };
	for (auto key : m_Keys)
		if (key.second) Move(KeyDirections.at(key.first) * dt * 5.f);
}

bool Player::Collided(Entity* other)
{
	return (dynamic_cast<Enemy*>(other));
}

Bullet Player::Shoot(sf::Vector2i MousePosition)
{
	return Bullet(this, NormalizedVector(static_cast<sf::Vector2f>(MousePosition) - this->GetPosition()));
}
