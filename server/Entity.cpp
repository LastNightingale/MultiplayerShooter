#include "Entity.h"
#include <iostream>

Entity::Entity(sf::RectangleShape Body) : m_Body(std::move(Body)) {}

void Entity::Move(Vector2f MoveVector)
{
	m_Body.move(MoveVector);
}

void Entity::Update(float dt)
{
	std::cout << "Entity" << std::endl;
}

void Entity::AddToRenderList(RenderList& list)
{
	list.Rects.push_back(m_Body);
}

Vector2f Entity::GetPosition() const
{
	return m_Body.getPosition();
}

void Entity::SetPosition(Vector2f pos)
{
	this->m_Body.setPosition(pos);
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
