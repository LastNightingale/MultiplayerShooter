#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "GameMath.hpp"
#define Bullet_Size 30
#define Bullet_Color Color::White

using namespace sf;

class Bullet : public Entity
{
private:
	//Player* m_Owner;
	Vector2f m_Direction;
public:
	Bullet(Player* owner, Vector2f direction);
	Bullet(Bullet&& other);
	Bullet();
	Bullet(const Bullet&) = default;
	void Update(float dt) override;
	bool Collided(Entity* other) override;
	//Player* GetOWner();
	Vector2f GetDirection() const;
	void SetDirection(Vector2f direction);
};

