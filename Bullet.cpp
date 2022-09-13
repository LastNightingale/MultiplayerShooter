#include "Bullet.h"
#include "GameMath.hpp"

Bullet::Bullet(Player* owner, Vector2f direction)
{
	m_Owner = owner;
	m_Direction = direction;
	m_Body.setSize({ 30, 30 });
	m_Body.setOrigin(VectorCenter(m_Body.getSize()));
	m_Body.setPosition(owner->GetPosition());
	m_Body.setFillColor(Color::White);
}

void Bullet::Update(float dt)
{
	m_Body.move(m_Direction * dt * 500.f);
}

