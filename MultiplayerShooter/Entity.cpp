#include "Entity.h"
#include <iostream>

void Entity::Move(Vector2f MoveVector)
{
	m_Body.move(MoveVector);
}

void Entity::SetStartPosition(Vector2f StartPosition)
{
	m_Body.setPosition(StartPosition);
}

void Entity::Update(float dt)
{
	std::cout << "Entity" << std::endl;
}

void Entity::Draw(RenderWindow& window)
{
	window.draw(m_Body);
}
