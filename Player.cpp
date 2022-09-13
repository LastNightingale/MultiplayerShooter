#include "Player.h"
#include <iostream>
#define Player_Size Vector2f(75, 75)

Player::Player()
{
	m_Body.setFillColor(Color::Blue);
	m_Body.setSize(Player_Size);
	m_Body.setOrigin(Vector2f(m_Body.getSize().x / 2, m_Body.getSize().y / 2));
	m_Body.setPosition(Vector2f(400, 400));
}

void Player::Update(float dt) 
{
	if (Keyboard::isKeyPressed(Keyboard::Key::A)) Move(Vector2f(-1000, 0) * dt);
	if (Keyboard::isKeyPressed(Keyboard::Key::D)) Move(Vector2f(1000, 0) * dt);
	if (Keyboard::isKeyPressed(Keyboard::Key::W)) Move(Vector2f(0, -1000) * dt);
	if (Keyboard::isKeyPressed(Keyboard::Key::S)) Move(Vector2f(0, 1000) * dt);
}
