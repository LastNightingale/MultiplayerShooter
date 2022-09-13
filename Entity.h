#pragma once
#include <SFML/Graphics.hpp>;
#include <iostream>

using namespace sf;

class Entity
{

protected:
	RectangleShape m_Body;
	int m_Lives = 0;
public:
	bool Exists = true;
public:
	Entity() = default;
	void Move(Vector2f MoveVector);

	virtual void Update(float dt);

	void Draw(RenderWindow& window);

	Vector2f GetPosition() const;

	RectangleShape GetBody();

	FloatRect GetGlobalBounds() const;

	virtual bool Collided(Entity* other);
};

