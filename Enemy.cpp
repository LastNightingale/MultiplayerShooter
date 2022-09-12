#include "Enemy.h"

Enemy::Enemy(Vector2f position, Vector2f direction)
{
	m_Body.setSize(Vector2f(50, 50));
	m_Body.setOrigin(VectorCenter(m_Body.getSize()));
	m_Body.setFillColor(Color::Red);
	m_Direction = direction;
	m_Body.setPosition(position);
}

void Enemy::Update(float dt)
{
	Move(m_Direction * dt * 200.f);
}
