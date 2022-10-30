#include "GameServer.h"

GameServer::GameServer()
{
	m_PlayerAmount = 2;
	m_ServerPort = 12500;
	m_GameStarted = false;
	m_ServerIsRunning = true;
	m_Socket.bind(m_ServerPort);
	m_Socket.setBlocking(true);
	m_Rect.setFillColor(sf::Color::Red);	
	m_Rect.setSize({ 100, 100 });
	m_Rect.setOrigin({ 50,50 });
	m_Rect.setPosition(0, 0);
	m_CurrentList.Rects.push_back(m_Rect);
}

void GameServer::ServerUpdate()
{
	m_CurrentList.Rects.clear();
	m_Rect.move( 1, 1 );
	m_CurrentList.Rects.push_back(m_Rect);
}

void GameServer::ServerSynchronize()
{
	AddConnection();
}

void GameServer::AddConnection()
{
	bool started;
	sf::Packet packet;
	unsigned short prt;
	sf::IpAddress ClientAdress;
	if (m_Socket.receive(packet, ClientAdress, prt) != sf::Socket::Done)
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
			pack << m_GameStarted << m_CurrentList;
			std::cout << "Packet size: " << pack.getDataSize() << std::endl;
			for (auto& connection : m_Connections)
			{
				m_Socket.send(pack, connection.IP, connection.Port);
			}
		}
		else
		{
			if (m_Connections.find(entryconnection) == m_Connections.end())
				m_Connections.insert(entryconnection);
			Check();
			DeliverStartGame();
		}
	}
}

void GameServer::Check()
{
	if (m_Connections.size() == m_PlayerAmount) m_GameStarted = true;
}

void GameServer::DeliverStartGame()
{
	sf::Packet pack;
	pack << m_GameStarted;
	for (auto& connection : m_Connections)
	{
		if (m_Socket.send(pack, connection.IP, connection.Port) != sf::Socket::Done)
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
		if (m_GameStarted) ServerUpdate();
		synchronthread.join();
	}
}