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
	m_Socket.setBlocking(false);
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
		if (started)
		{
			for (auto& connection : m_Connections)
			{
				if (!(entryconnection == connection))
					m_Socket.send(packet, connection.IP, connection.Port);
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
		else std::cout << "aaaaaa\n";
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
