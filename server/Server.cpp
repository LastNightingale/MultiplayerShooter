#include "Server.h"
#include <iostream>

Server::Server()
{
	m_PlayerAmount = 2;
	m_Port = 12500;
	m_Port2 = 12501;
	m_GameStarted = false;
	m_ServerIsRunning = true;
	m_Socket.bind(m_Port);
	m_Socket.setBlocking(true);
}

void Server::AddConnection()
{
	bool started;
	sf::Packet packet;
	unsigned short prt;
	sf::IpAddress ClientAdress;
	if (m_Socket.receive(packet, ClientAdress, prt) != sf::Socket::Done)
	{
		std::cout << "No new connections\n";
	}
	else
	{
		Connection entryconnection(ClientAdress, prt);
		packet >> started;
		//std::cout << started << "\n";
		if (started)
		{
			for (auto& connection : m_Connections)
			{
				std::cout << entryconnection.IP << entryconnection.Port << "   " << connection.IP << connection.Port << "\n";
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
					std::cout << std::endl;*/
				}
					
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
	/*std::cout << m_Connections.size() << std::endl;
	std::cout << ClientAdress.toString() << std::endl;*/
}

void Server::Check()
{
	if (m_Connections.size() == m_PlayerAmount) m_GameStarted = true;
}

void Server::DeliverPackets()
{
	
}

void Server::DeliverStartGame()
{
	sf::Packet pack;
	pack << m_GameStarted;
	for (auto& connection : m_Connections)
	{
		if (m_Socket.send(pack, connection.IP, connection.Port) != sf::Socket::Done)
			std::cout << "Packet haven't been delivered\n";
		//else std::cout << "aaaaaa\n";
	}
}

void Server::DeliverData(sf::Packet& packet)
{

}

void Server::Run()
{
	while (m_ServerIsRunning)
	{
		/*if (m_GameStarted) DeliverPackets();
		else AddConnection();*/
		AddConnection();
	}
}
