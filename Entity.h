#pragma once
#include <SFML/Graphics.hpp>;

using namespace sf;

class Entity
{
protected:
	RectangleShape m_Body;
	int m_Lives = 0;
public:
	Entity() {};
	void Move(Vector2f MoveVector);
	
	void SetStartPosition(Vector2f StartPosition);

	virtual void Update(float dt);

	void Draw(RenderWindow& window);
};

