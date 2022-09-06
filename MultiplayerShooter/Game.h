#pragma once
#include <vector>
#include "Enemy.h"
#include "Player.h"
#include <thread>
#include "GameMath.cpp"

using namespace std;

class Game
{
private:
	RenderWindow m_Window{ VideoMode(1400,900), "Shooter", sf::Style::Close };
	vector<Entity*> m_Entities;
public:
	Game();
	//void SpawnEnemy();
	void GameUpdate(float dt);
	void GameDraw();
	void Run();
};

