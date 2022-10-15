#include "Game.h"
#include <iostream>

Game::Game()
{	
	m_Entities.push_back(new Player());
	m_ClientPlayer = reinterpret_cast<Player*>(m_Entities[0]);
	m_GameStarted = false; // ������
	m_isRunning = true;
	m_DataDelivered = false; // ������
	m_Dt = m_Spawntime = 0;
	m_ServerPort = 12500;
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
		RecieveEntities();
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

		/*for (auto& iter : m_OtherEntities)
		{
			iter->Update(m_Dt);
		}*/

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
		for (auto& entity = m_OtherEntities.rbegin(); entity != m_OtherEntities.rend(); entity++)
		{
			(*entity)->AddToRenderList(list);
		}
		m_DrawLock.lock();
		m_CurrentList = list;
		m_DrawLock.unlock();
		DeliverEntities();
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
	while (true)
	{
		sf::Packet pack;
		if (!m_DataDelivered)
		{			
			std::cout << "Enter server IP:\n";
			std::cin >> m_ServerIP;
			std::cout << "Enter port:\n";
			std::cin >> m_ClientPort;
			m_Socket.bind(m_ClientPort);
		}
		pack << m_GameStarted;
		if (m_Socket.send(pack, m_ServerIP, m_ServerPort) == sf::Socket::Done)
		{
			m_DataDelivered = true;
			std::cout << "Delivered\n";
		}
		sf::Packet StartGamePacket;
		IpAddress ServerAddress;
		unsigned short ClientPort;
		if (m_Socket.receive(StartGamePacket, ServerAddress, ClientPort) == Socket::Done)
		{
			StartGamePacket >> m_GameStarted;
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

void Game::DeliverEntities()
{
	sf::Packet EntityPacket;
	std::vector<Enemy> enemies;
	std::vector<Bullet> bullets;
	std::vector<Player> players;
	for (auto entity : m_Entities)
	{
		if (dynamic_cast<Player*>(entity)) players.push_back(Player(dynamic_cast<Player&>(*entity)));
		if (dynamic_cast<Enemy*>(entity)) enemies.push_back(Enemy(dynamic_cast<Enemy&>(*entity)));
		if (dynamic_cast<Bullet*>(entity)) bullets.push_back(Bullet(dynamic_cast<Bullet&>(*entity)));
	}
	
	EntityPacket << m_GameStarted << players << enemies << bullets;

	if (m_Socket.send(EntityPacket, m_ServerIP, m_ServerPort) == sf::Socket::Done)
	{
		//aaa
	}
}

void Game::RecieveEntities()
{
	sf::Packet EntitiesPacket;
	IpAddress ServerAddress;
	unsigned short ClientPort;
	if (m_Socket.receive(EntitiesPacket, ServerAddress, ClientPort) == Socket::Done)
	{
		m_OtherEntities.clear();
		std::vector<Enemy> enemies;
		std::vector<Bullet> bullets;
		std::vector<Player> players;
		bool temp;
		EntitiesPacket >> players >> enemies >> bullets;
		for (auto player : players)
		{
			m_OtherEntities.push_back(new Player(player));
		}		
		for (auto enemy : enemies)
		{
			m_OtherEntities.push_back(new Enemy(enemy));
		}
		for (auto bullet : bullets)
		{
			m_OtherEntities.push_back(new Bullet(bullet));
		}
	}
}

