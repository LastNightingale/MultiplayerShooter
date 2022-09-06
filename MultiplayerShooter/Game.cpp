#include "Game.h"

Game::Game()
{	
	m_Entities.push_back(new Player());
}

void Game::GameUpdate(float dt)
{
	for (auto iter : m_Entities)
	{
		iter->Update(dt);
	}
}

void Game::GameDraw()
{
	m_Window.clear(Color::Black);
	for (auto iter : m_Entities)
	{
		iter->Draw(m_Window);
	}
	m_Window.display();
}

void Game::Run()
{
	Clock clock;
	float dt, spawntime;	
	while (m_Window.isOpen())
	{
		dt = clock.getElapsedTime().asSeconds();
		clock.restart();
		Event event;
		while (m_Window.pollEvent(event))
		{
			spawntime += dt;
			if (spawntime >= 2.f) SpawnEnemy();
		}
		thread updatethread([this, dt]
			{
				return this->GameUpdate(dt);
			}); 
		GameDraw();
		updatethread.join();
	}
}
