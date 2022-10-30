#include "Bullet.h"

class Enemy; 

Bullet::Bullet(Player* owner, Vector2f direction)
{
	//m_Owner = owner;
	m_Direction = direction;
	m_Body.setSize({ Bullet_Size, Bullet_Size });
	m_Body.setOrigin(VectorCenter(m_Body.getSize()));
	m_Body.setPosition(owner->GetPosition());
	m_Body.setFillColor(Bullet_Color);
}

Bullet::Bullet(Bullet&& other) : Entity(other.m_Body), m_Direction(std::move(other.m_Direction))
/*m_Owner(std::move(other.m_Owner)*/ {}

Bullet::Bullet()
{
	//m_Owner = owner;
	m_Body.setSize({ Bullet_Size, Bullet_Size });
	m_Body.setOrigin(VectorCenter(m_Body.getSize()));
	m_Body.setFillColor(Bullet_Color);
}

void Bullet::Update(float dt)
{
	m_Body.move(m_Direction * dt * 500.f);
}

bool Bullet::Collided(Entity* other)
{
	return (dynamic_cast<Enemy*>(other));
}

//Player* Bullet::GetOWner()
//{
//	return m_Owner;
//}

Vector2f Bullet::GetDirection() const
{
	return this->m_Direction;
}

void Bullet::SetDirection(Vector2f direction)
{
	this->m_Direction = direction;
}

