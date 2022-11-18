#include "GameServer.h"

GameServer::GameServer()
{
	m_CurrentPosition = 0;
	m_MaxPlayerAmount = 1;
	m_ServerPort = 12500;
	m_GameStarted = false;
	m_ServerIsRunning = true;
	m_UdpSocket.bind(m_ServerPort);
	m_TcpListener.listen(m_ServerPort + (unsigned short)50);
	m_UdpSocket.setBlocking(true);
	m_Dt = m_Spawntime = 0;
	/*m_Rect.setFillColor(sf::Color::Red);
	m_Rect.setSize({ 100, 100 });
	m_Rect.setOrigin({ 50,50 });
	m_Rect.setPosition(0, 0);
	m_CurrentList.Rects.push_back(m_Rect);*/
}

void GameServer::InitPlayer()
{
	std::vector<Entity*> vector;
	m_PlayerLock.lock();
	vector = m_Entities;
	m_PlayerLock.unlock();
	vector.push_back(new Player());
	Entity* temp = vector[m_Connections.size() - 1];
	vector[m_Connections.size() - 1] = vector[vector.size() - 1];
	vector[vector.size() - 1] = temp;
	m_PlayerLock.lock();
	m_Entities = vector;
	m_PlayerLock.unlock();
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

void GameServer::ServerUpdate()
{
	while (m_ServerIsRunning)
	{
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
		//std::cout << "Spawntime : " << m_Spawntime << std::endl;
		if (m_Spawntime >= 1.5)
		{			
			InitEnemy();
			m_Spawntime = 0;
		}
		for (auto& iter : m_Entities)
		{
			iter->Update(m_Dt);
		}

		for (auto& element : m_Events)
		{			
			for (auto& event : element.second.Events)
			{
				if (event.type == sf::Event::EventType::MouseButtonPressed && event.key.code == sf::Mouse::Button::Left)
				{
					m_Entities.push_back(new Bullet(m_Players[element.first]->Shoot(element.second.ScreenPosition)));
					//std::cout << "Bullet \n";
				}
			}
		}
			
		//std::cout << "Updated"  << std::endl;
		/*m_CurrentList.Rects.clear();
		m_Rect.move( 1, 1 );
		m_CurrentList.Rects.push_back(m_Rect);*/
		RenderList list;
		for (auto& entity : m_Entities)
			entity->AddToRenderList(list);
		//std::cout << "ListToDraw : " << list.Rects.size() << std::endl;
		m_SynchronLock.lock();
		m_CurrentList = list;
		m_SynchronLock.unlock();
	}
	
}

void GameServer::ServerSynchronize()
{
	while (m_ServerIsRunning)
	{
		AddConnection();
	}	
}

void GameServer::ServerEvents()
{
	while (m_ServerIsRunning)
	{
		std::cout << "Start\n";
		sf::Packet packet;	
		//m_TcpListener.listen(m_ServerPort + (unsigned short)50);		
		std::cout << "Point\n";
		/*if(m_GameStarted) */
		if (m_TcpListener.accept(m_TcpSocket) != sf::Socket::Status::Done)
		{
			std::cout << "Accept Error\n";
		}
		//std::cout << "After accept\n";
		if (m_TcpSocket.receive(packet) == sf::Socket::Done)
		{
			sf::IpAddress ip;
			int port;
			ScreenEvent scevent;
			packet >> port >> scevent;
			if (scevent.Events.size() > 0)
			{
				std::cout << "Events to recieve : " << scevent.Events.size() << std::endl;
				std::cout << "All entities : " << m_Entities.size() << std::endl;
			}
			//std::cout << ip.toString() << " " << port << " " << scevent.Events.size() << std::endl;
			Connection connection(m_TcpSocket.getRemoteAddress(), static_cast<unsigned short>(port));
			//std::cout << "Port: " << m_TcpSocket.getRemotePort() << std::endl;
			m_EventLock.lock();
			m_Events[connection] = scevent;
			m_EventLock.unlock();
		}
		else std::cout << "Didn't recieve TCP\n";
		//m_TcpListener.close();
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
		//std::cout << started << "\n";
		if (started)
		{		
			/*for (auto& connection : m_Connections)
			{
				//std::cout << entryconnection.IP << entryconnection.Port << "   " << connection.IP << connection.Port << "\n";
				if (!(entryconnection == connection))
				{
					//std::cout << packet.getDataSize() << std::endl;
					m_Socket.send(packet, connection.IP, connection.Port);
					//std::cout << "Client " << connection.Port << " " << packet.getDataSize() << std::endl;	
					/*std::cout << "Packet\n";
					const uint8_t* ptr = static_cast<const uint8_t*>(packet.getData());
					for (size_t i = 0; i < packet.getDataSize(); ++i)
					{
						std::cout << std::hex << (uint32_t)ptr[i] << " ";
					}
					std::cout << std::endl;
				}*/
			sf::Packet pack;
			RenderList list;			
			m_SynchronLock.lock();
			list = m_CurrentList;
			m_SynchronLock.unlock();
			pack << m_GameStarted << list;
			//std::cout << "Packet size: " << pack.getDataSize() << std::endl;
			for (auto& connection : m_Connections)
			{
				m_UdpSocket.send(pack, connection.IP, connection.Port);
			}
		}
		else
		{			
			if (m_Connections.find(entryconnection) == m_Connections.end())
			{
				//std::cout << entryconnection.IP.toString() << std::endl;
				m_Connections.insert(entryconnection);
				InitPlayer(); 	
				//std::cout << "VectorToDraw : " << m_Entities.size() << std::endl;
				m_Players.insert(std::pair<Connection, Player*>(entryconnection,
					reinterpret_cast<Player*>(m_Entities[m_Connections.size() - 1])));
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
			//std::cout << "Packet haven't been delivered\n";
		//else std::cout << "aaaaaa\n";
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
