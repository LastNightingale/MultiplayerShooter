#include "Game.h"
#include <iostream>

Game::Game()
{	
	m_Entities.push_back(new Player());
}

void Game::SpawnEnemy()
{
	int side = rand() % 4 + 1;
	Vector2f spawnvector;
	switch (side)
	{
	case 1:
		spawnvector = Vector2f(rand() % m_Window.getSize().x, 0);
		break;
	case 2:
		spawnvector = Vector2f(0, rand() % m_Window.getSize().y);
		break;
	case 3:
		spawnvector = Vector2f(rand() % m_Window.getSize().x, m_Window.getSize().y);
		break;
	case 4:
		spawnvector = Vector2f(m_Window.getSize().x, rand() % m_Window.getSize().y);
		break;		
	}
	m_Entities.push_back(new Enemy(spawnvector,
		NormalizedVector(VectorCenter(static_cast<Vector2f>(m_Window.getSize())) - spawnvector)));
}

void Game::GameUpdate(float dt)
{
	for (auto iter : m_Entities)
	{
		iter->Update(dt);
	}

	for (Entity *outer : m_Entities)
	{
		for (Entity* inner : m_Entities)
		{
			if (outer->GetGlobalBounds().findIntersection(inner->GetGlobalBounds()))
			{
				if (outer != inner && typeid(*outer) != typeid(*inner))
				{
					m_DestroyedEntities.push_back(inner);
				}				
			}				
		}
	}

	for (Entity *entity : m_DestroyedEntities)
	{
		for (int i = 0; i < m_Entities.size(); i++)
		{
			if (entity == m_Entities[i])
			{
				Entity* temp = m_Entities[i];
				m_Entities[i] = m_Entities[m_Entities.size() - 1];
				m_Entities[m_Entities.size() - 1] = temp;
				m_Entities.pop_back();
				delete temp;
			}
		}
	}
	m_DestroyedEntities.clear();
}

void Game::GameDraw()
{
	m_Window.clear(Color::Black);
	for (auto iter = m_Entities.rbegin(); iter != m_Entities.rend(); ++iter)
	{
		(*iter)->Draw(m_Window);
	}
	m_Window.display();
}

void Game::Run()
{
	srand(time(NULL));
	Clock clock;
	float dt, spawntime = 0;
	while (m_Window.isOpen())
	{
		dt = clock.getElapsedTime().asSeconds();
		clock.restart();
		Event event;
		while (m_Window.pollEvent(event))
		{
			if (event.type == sf::Event::EventType::Closed) m_Window.close();
			if (event.type == sf::Event::EventType::MouseButtonReleased && event.key.code == sf::Mouse::Button::Left)
			{
				m_Entities.push_back(new Bullet(reinterpret_cast<Player*>(m_Entities[0]),
					NormalizedVector(static_cast<Vector2f>(Mouse::getPosition(m_Window)) - m_Entities[0]->GetPosition())));
			}
		}
		spawntime += dt;
		if (spawntime >= 2.f)
		{
			SpawnEnemy();
			spawntime = 0;
		}
		thread updatethread([this, dt]
			{
				return this->GameUpdate(dt);
			}); 
		GameDraw();
		updatethread.join();
	}
}
