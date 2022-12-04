#include "GameServer.h"

GameServer::GameServer()
{
	m_CurrentPosition = 0;
	m_MaxPlayerAmount = 2;
	m_ServerPort = 12500;
	m_GameStarted = m_Connected = false;
	m_ServerIsRunning = true;
	m_UdpSocket.bind(m_ServerPort);
	m_TcpListener.listen(m_ServerPort + (unsigned short)50);
	m_Selector.add(m_TcpListener);
	m_UdpSocket.setBlocking(true);
	m_Dt = m_Spawntime = 0;
	m_Keys.insert({ sf::Keyboard::Key::D, {100.f, 0.f} });
	m_Keys.insert({ sf::Keyboard::Key::A, {-100.f, 0.f} });
	m_Keys.insert({ sf::Keyboard::Key::W, {0.f, -100.f} });
	m_Keys.insert({ sf::Keyboard::Key::S, {0.f, 100.f} });
}

void GameServer::TestConnect()
{
	if (m_TcpListener.accept(m_TestSocket) == sf::Socket::Status::Done)
		m_Connected = true;
}

void GameServer::InitPlayer(Connection connection)
{
	m_SynchronLock.lock();
	m_Entities.push_back(new Player());
	Entity* temp = m_Entities[m_Connections.size() - 1];
	m_Entities[m_Connections.size() - 1] = m_Entities[m_Entities.size() - 1];
	m_Entities[m_Entities.size() - 1] = temp;
	m_Players.insert(std::pair<Connection, Player*>(connection,
		reinterpret_cast<Player*>(m_Entities[m_Connections.size() - 1])));
	m_SynchronLock.unlock();
}

void GameServer::DeletePlayer(Player* lost)
{	
	for (auto& player : m_Players)
	{
		if (lost == player.second)
		{
			m_Players.erase(player.first);
			break;
		}			
	}	
}

void GameServer::InitEnemy()
{
	int side = rand() % 4 + 1;
	Vector2f spawnvector;
	switch (side)
	{
	case 1:
		spawnvector = Vector2f(rand() % m_WindowSize.x, 0);
		break;
	case 2:
		spawnvector = Vector2f(0, rand() % m_WindowSize.y);
		break;
	case 3:
		spawnvector = Vector2f(rand() % m_WindowSize.x, m_WindowSize.y);
		break;
	case 4:
		spawnvector = Vector2f(m_WindowSize.x, rand() % m_WindowSize.y);
		break;
	}
	m_Entities.push_back(new Enemy(spawnvector,
		NormalizedVector(m_Entities[IteratePlayer()]->GetPosition() - spawnvector)));
}

int GameServer::IteratePlayer()
{
	if (m_CurrentPosition == m_Connections.size() - 1) m_CurrentPosition = 0;
	else ++m_CurrentPosition;
	return m_CurrentPosition;
}

void GameServer::Collision()
{
	for (Entity* entity : m_DestroyedEntities)
	{
		for (int i = 0; i < m_Entities.size(); i++)
		{
			if (entity == m_Entities[i])
			{
				if (Player* lost = dynamic_cast<Player*>(entity)) DeletePlayer(lost);					
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

void GameServer::ServerUpdate()
{
	while (m_ServerIsRunning)
	{
		ZoneScopedN("ServerUpdate");
		if (m_GameStarted)
		{
			m_Dt = m_Clock.getElapsedTime().asSeconds();
			m_Clock.restart();
			m_Spawntime += m_Dt;			
		}	
		else
		{
			m_Clock.restart();
		}
		RenderList list;
		m_SynchronLock.lock();
		if (m_Spawntime >= 1.5)
		{			
			InitEnemy();
			m_Spawntime = 0;
		}

		m_EventLock.lock();
		for (auto& element : m_Events)
		{
			for (auto& event : element.second.Events)
			{
				if (event.type == sf::Event::EventType::MouseButtonPressed && event.key.code == sf::Mouse::Button::Left)
				{
					m_Entities.push_back(new Bullet(m_Players[element.first]->Shoot(element.second.ScreenPosition)));
				}
				if (event.type == sf::Event::EventType::KeyPressed && m_Keys.find(event.key.code) != m_Keys.end())
				{
					m_Players.at(element.first)->m_Keys.at(event.key.code) = true;
				}
				if (event.type == sf::Event::EventType::KeyReleased && m_Keys.find(event.key.code) != m_Keys.end())
				{
					m_Players.at(element.first)->m_Keys.at(event.key.code) = false;
				}
			}
		}
		m_Events.clear();
		m_EventLock.unlock();

		for (auto& iter : m_Entities)
		{
			if (abs(iter->GetPosition().x - Window_Size.x / 2) > Window_Size.x / 2 + 50.f ||
				abs(iter->GetPosition().y - Window_Size.y / 2) > Window_Size.y / 2 + 50.f)
				m_DestroyedEntities.push_back(iter);
		}

		Collision();

		for (Entity* outer : m_Entities)
		{
			for (Entity* inner : m_Entities)
			{
				if (outer->GetGlobalBounds().intersects(inner->GetGlobalBounds()))
				{
					if (outer->Collided(inner))
					{
						m_DestroyedEntities.push_back(inner);
						//std::cout << "To Destroy :" << m_DestroyedEntities.size() << std::endl;
					}
				}
			}
		}

		Collision();

		for (auto& iter : m_Entities)
		{
			iter->Update(m_Dt);
		}
		
				
		/*for (auto& entity : m_Entities)
			entity->AddToRenderList(list);*/
		for (auto& entity = m_Entities.rbegin(); entity != m_Entities.rend(); entity++)
		{
			(*entity)->AddToRenderList(list);
		}
		//std::cout << "ListToDraw : " << list.Rects.size() << std::endl;		
		m_CurrentList = list;
		m_SynchronLock.unlock();
	}
	
}

void GameServer::ServerSynchronize()
{
	while (m_ServerIsRunning)
	{
		ZoneScopedN("ServerSynchron");
		AddConnection();
	}	
}

void GameServer::ServerEvents()
{
	while (m_ServerIsRunning)
	{
		ZoneScopedN("ServerEvent");		
		if (m_Selector.wait())
		{
			if (m_Selector.isReady(m_TcpListener) && m_Clients.size() < m_MaxPlayerAmount)
			{
				sf::TcpSocket* client = new sf::TcpSocket;
				if (m_TcpListener.accept(*client) == sf::Socket::Done)
				{
					m_Clients.push_back(client);
					m_Selector.add(*client);
				}
				else
				{
					delete client;
				}
			}
			else
			{
				for (auto it : m_Clients)
				{
					sf::TcpSocket& client = *it;
					if (m_Selector.isReady(client))
					{
						sf::Packet packet;
						if (client.receive(packet) == sf::Socket::Done)
						{
							sf::IpAddress ip;
							int port;
							ScreenEvent scevent;
							bool alive = false;
							packet >> port >> scevent;
							Connection connection(client.getRemoteAddress(), static_cast<unsigned short>(port));
							m_SynchronLock.lock();
							if (m_Players.find(connection) != m_Players.end()) alive = true;
							m_SynchronLock.unlock();
							m_EventLock.lock();							
							if(alive) m_Events[connection] = scevent;							
							m_EventLock.unlock();
							
							//std::cout << "End of recieving events marker\n";
						}
						//else std::cout << "Didn't recieve TCP\n";
					}
				}
			}
		}		
	}
}

void GameServer::AddConnection()
{
	bool started;
	sf::Packet packet;
	unsigned short prt;
	sf::IpAddress ClientAdress;
	if (m_UdpSocket.receive(packet, ClientAdress, prt) != sf::Socket::Done)
	{
		//std::cout << "No new connections\n";
	}
	else
	{
		Connection entryconnection(ClientAdress, prt);
		packet >> started;
		if (started)
		{		
			sf::Packet pack;
			RenderList list;	
			bool isPlayerDead;
			m_SynchronLock.lock();
			list = m_CurrentList;
			m_SynchronLock.unlock();
			pack << m_GameStarted << list;
			for (auto& connection : m_Connections)
			{
				m_UdpSocket.send(pack, connection.IP, connection.Port);
			}
		}
		else
		{			
			if (m_Connections.find(entryconnection) == m_Connections.end())
			{
				m_Connections.insert(entryconnection);
				InitPlayer(entryconnection);							
			}				
			Check();
			DeliverStartGame();
		}
	}
}

void GameServer::Check()
{
	if (m_Connections.size() == m_MaxPlayerAmount) m_GameStarted = true;
}

void GameServer::DeliverStartGame()
{
	sf::Packet pack;
	pack << m_GameStarted;
	for (auto& connection : m_Connections)
	{
		if (m_UdpSocket.send(pack, connection.IP, connection.Port) != sf::Socket::Done)
		{

		}
	}
}

void GameServer::Run()
{
	while (m_ServerIsRunning)
	{
		std::thread synchronthread([this]
			{
				ServerSynchronize();
			});
		std::thread eventthread([this]
			{
				ServerEvents();
			});
		ServerUpdate();
		synchronthread.join();
		eventthread.join();
	}	
}
