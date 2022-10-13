#include "Game.h"
#include <iostream>

Game::Game()
{	
	m_Entities.push_back(new Player());
	m_ClientPlayer = reinterpret_cast<Player*>(m_Entities[0]);
	m_GameStarted = true; // змінити
	m_isRunning = true;
	m_DataDelivered = true; // змінити
	m_Dt = m_Spawntime = 0;
	m_Port = 12500;
	m_Port2 = 12501;
	m_Socket.setBlocking(false);
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
		NormalizedVector(m_ClientPlayer->GetPosition() - spawnvector)));
}

void Game::GameUpdate(float dt)
{
	while (m_isRunning)		
	{
		m_DrawStarted.Wait();		
		m_Spawntime += m_Dt;
		if (m_Spawntime >= 2.f)
		{
			SpawnEnemy();
			m_Spawntime = 0;
		}
		std::vector<Event> events;
		m_EventLock.lock();
		events = m_Events;
		m_EventLock.unlock();
		for (auto& event : m_Events)
		{
			if (event.type == sf::Event::EventType::Closed)
			{
				m_Window.close();
				m_isRunning = false;
			}
			if (event.type == sf::Event::EventType::MouseButtonReleased && event.key.code == sf::Mouse::Button::Left)
			{
				m_Entities.push_back(new Bullet(m_ClientPlayer->Shoot(Mouse::getPosition(m_Window))));
			}
		}
		for (auto& iter : m_Entities)
		{
			iter->Update(m_Dt);
		}

		for (Entity* outer : m_Entities)
		{
			for (Entity* inner : m_Entities)
			{
				if (outer->GetGlobalBounds().intersects(inner->GetGlobalBounds()))
				{
					if (outer->Collided(inner))
					{
						m_DestroyedEntities.push_back(inner);
					}
				}
			}
		}
		Collision();
		RenderList list;
		for (auto& entity = m_Entities.rbegin(); entity != m_Entities.rend(); entity++)
		{
			(*entity)->AddToRenderList(list);
		}
		m_DrawLock.lock();
		m_CurrentList = list;
		m_DrawLock.unlock();
	}	
}
void Game::Collision()
{
	for (Entity* entity : m_DestroyedEntities)
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
				break;
			}
		}
	}
	m_DestroyedEntities.clear();
}

void Game::GameDraw()
{
	while (m_Window.isOpen())
	{
		m_DrawStarted.Signal();
		m_Dt = m_Clock.getElapsedTime().asSeconds();
		m_Clock.restart();		
		RenderList list;
		m_DrawLock.lock();
		list = m_CurrentList;
		m_DrawLock.unlock();
		m_Window.clear(Color::Black);
		for (auto& rect : list.Rects)
		{
			m_Window.draw(rect);
		}
		m_Window.display();
		Event event;
		std::vector<Event> events;
		while (m_Window.pollEvent(event))
		{
			events.push_back(event);			
		}
		m_EventLock.lock();
		m_Events = events;
		m_EventLock.unlock();
	}	
	m_isRunning = false;
}

void Game::Run()
{
	srand(time(NULL));	
	std::string ip;
	unsigned short port;
	while (true)
	{
		sf::Packet pack;
		if (!m_DataDelivered)
		{
			
			std::cout << "Enter IP:\n";
			std::cin >> ip;
			std::cout << "Enter port:\n";
			std::cin >> port;
			m_Socket.bind(port);
		}
		
		if (m_Socket.send(pack, ip, m_Port) == sf::Socket::Done)
		{
			m_DataDelivered = true;
			std::cout << "Delivered\n";
		}
		sf::Packet packe;
		IpAddress adr;
		unsigned short prt;
		if (m_Socket.receive(packe, adr, prt) == Socket::Done)
		{
			packe >> m_GameStarted;
			std::cout << "Packet recieved\n";
		}
		if (m_GameStarted)
		{			
			thread updatethread([this]
				{
					GameUpdate(m_Dt);
				});
			GameDraw();
			updatethread.join();		
		}	

	}
}
