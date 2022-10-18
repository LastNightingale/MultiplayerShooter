#include "Game.h"
#include <iostream>

Game::Game()
{	
	m_Entities.push_back(new Player());
	m_ClientPlayer = reinterpret_cast<Player*>(m_Entities[0]);
	m_GameStarted = false; // змінити
	m_isRunning = true;
	m_isSynchronized = true;
	m_DataDelivered = false; // змінити
	m_Dt = m_Spawntime = 0;
	m_ServerPort = 12500;
	m_Socket.setBlocking(true);
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
		//RecieveEntities();
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
		std::vector<Entity*> others;
		m_SynchronLock.lock();
		others = m_OtherEntities;
		m_SynchronLock.unlock();
		for (auto& entity = m_Entities.rbegin(); entity != m_Entities.rend(); entity++)
		{
			(*entity)->AddToRenderList(list);
		}
		for (auto& entity = others.rbegin(); entity != others.rend(); entity++)
		{
			(*entity)->AddToRenderList(list);
		}
		m_DrawLock.lock();
		m_CurrentList = list;
		m_DrawLock.unlock();
		//DeliverEntities();
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
			thread synchronizethread([this]
				{
					GameSynchronize();
				});
			GameDraw();
			updatethread.join();
			synchronizethread.join();
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
	
	EntityPacket << true << players << enemies << bullets;

	
	if (m_Socket.send(EntityPacket, m_ServerIP, m_ServerPort) != sf::Socket::Done)
	{
		std::cout << "Help me" << std::endl;
	}
	else cout << "Deliver " << this->m_ClientPort << " " << EntityPacket.getDataSize() << endl;
}

void Game::RecieveEntities()
{
	sf::Packet EntitiesPacket;
	IpAddress ServerAddress;
	unsigned short ClientPort;
	if (m_Socket.receive(EntitiesPacket, ServerAddress, ClientPort) == Socket::Done)
	{		
		std::vector<Enemy> enemies;
		std::vector<Bullet> bullets;
		std::vector<Player> players;
		bool temp;
		cout << "Recieve " << this->m_ClientPort << " " << EntitiesPacket.getDataSize() << endl;
		EntitiesPacket >> temp >> players >> enemies >> bullets;
		
		std::vector<Entity*> others;		
		for (auto player : players)
		{
			others.push_back(new Player(player));
		}
		for (auto enemy : enemies)
		{
			others.push_back(new Enemy(enemy));
		}
		for (auto bullet : bullets)
		{
			others.push_back(new Bullet(bullet));
		}
		cout << players.size() << " " << enemies.size() << " " << bullets.size() << endl;
		m_SynchronLock.lock();
		m_OtherEntities = others;
		m_SynchronLock.unlock();		
	}
	else cout << "Help with recieve" << endl;

}

void Game::GameSynchronize()
{
	while (m_isSynchronized)
	{
		DeliverEntities();
		RecieveEntities();
	}
}

