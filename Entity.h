#pragma once
#include <SFML/Graphics.hpp>;
#include <vector>
#include <iostream>

using namespace sf;

struct RenderList
{
	std::vector<RectangleShape> Rects;
};

class Entity
{

protected:
	RectangleShape m_Body;
	int m_Lives = 0;
public:
	bool Exists = true;
public:
	Entity() = default;
	Entity(sf::RectangleShape Body);
	void Move(Vector2f MoveVector);

	virtual void Update(float dt);

	void AddToRenderList(RenderList& list);

	Vector2f GetPosition() const;

	RectangleShape GetBody();

	FloatRect GetGlobalBounds() const;

	virtual bool Collided(Entity* other);
};

