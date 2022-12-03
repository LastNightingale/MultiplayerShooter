#include "Enemy.h"

Enemy::Enemy(Vector2f position, Vector2f direction)
{
	m_Body.setSize(Vector2f(Enemy_Size, Enemy_Size));
	m_Body.setOrigin(VectorCenter(m_Body.getSize()));
	m_Body.setFillColor(Enemy_Color);
	m_Direction = direction;
	m_Body.setPosition(position);
}

Enemy::Enemy()
{
	m_Body.setSize(Vector2f(Enemy_Size, Enemy_Size));
	m_Body.setOrigin(VectorCenter(m_Body.getSize()));
	m_Body.setFillColor(Enemy_Color);
}

void Enemy::Update(float dt)
{
	Move(m_Direction * dt * 200.f);
}

bool Enemy::Collided(Entity* other)
{
	if (dynamic_cast<Bullet*>(other)) std::cout << "Enemy destroyed bullet\n";
	return (!dynamic_cast<Enemy*>(other));
	//else return true;
}

Vector2f Enemy::GetDirection() const
{
	return this->m_Direction;
}

void Enemy::SetDirection(Vector2f direction)
{
	this->m_Direction = direction;
}
