#include "Entity.h"
#include <iostream>

void Entity::Move(Vector2f MoveVector)
{
	m_Body.move(MoveVector);
}

void Entity::Update(float dt)
{
	std::cout << "Entity" << std::endl;
}

void Entity::Draw(RenderWindow& window)
{
	window.draw(m_Body);
}

Vector2f Entity::GetPosition() const
{
	return m_Body.getPosition();
}

RectangleShape Entity::GetBody()
{
	return m_Body;
}

FloatRect Entity::GetGlobalBounds() const 
{
	return m_Body.getGlobalBounds();
}

bool Entity::Collided(Entity* other)
{
	return false;
}
