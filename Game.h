#pragma once
#include <vector>
#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"
#include <thread>
#include "GameMath.hpp"

using namespace std;

class Game
{
private:
	RenderWindow m_Window{ VideoMode({ 1400,900 }) , "Shooter", sf::Style::Close};
	vector<Entity*> m_Entities;
	vector<Entity*> m_DestroyedEntities;
public:
	Game();
	void SpawnEnemy();
	void GameUpdate(float dt);
	void GameDraw();
	void Run();
};

