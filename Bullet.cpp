#include "Bullet.h"

Bullet::Bullet(Player* owner, Vector2f direction)
{
	m_Owner = owner;
	m_Direction = direction;
	m_Body.setSize({ 30, 30 });
	m_Body.setOrigin(VectorCenter(m_Body.getSize()));
	m_Body.setPosition(owner->GetPosition());
	m_Body.setFillColor(Color::White);
}

Bullet::Bullet(Bullet&& other) : Entity(other.m_Body), m_Direction(std::move(other.m_Direction)),
m_Owner(std::move(other.m_Owner)) {}

void Bullet::Update(float dt)
{
	m_Body.move(m_Direction * dt * 500.f);
}

bool Bullet::Collided(Entity* other)
{
	if (Player* player = dynamic_cast<Player*>(other))
	{
		if (player == this->GetOWner()) return false;
		else return true;
	}
	else if (dynamic_cast<Bullet*>(other)) return false;
	else return true;
}

Player* Bullet::GetOWner()
{
	return m_Owner;
}

