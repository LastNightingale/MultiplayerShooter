#pragma once
#include <SFML/Graphics.hpp>
#include "RenderList.hpp"

using namespace sf;

class Entity
{
protected:
	RectangleShape m_Body;
public:
	Entity() = default;
	Entity(sf::RectangleShape Body);
	void Move(Vector2f MoveVector);

	virtual void Update(float dt);

	void AddToRenderList(RenderList& list);

	Vector2f GetPosition() const;

	void SetPosition(Vector2f pos);

	RectangleShape GetBody();

	FloatRect GetGlobalBounds() const;

	virtual bool Collided(Entity* other);
};
