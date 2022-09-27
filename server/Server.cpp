#include "Server.h"
#include <iostream>

Server::Server()
{
	m_PlayerAmount = 0;
	m_Port = 12500;
	m_Port2 = 12501;
	m_GameStarted = false;
	m_Socket.bind(m_Port);
	m_Socket.setBlocking(false);
}

void Server::AddConnection()
{
	sf::Packet packet;
	unsigned short prt;
	sf::IpAddress ClientAdress;
	if (m_Socket.receive(packet, ClientAdress, prt) != sf::Socket::Done)
	{
		std::cout << "No new connections\n";
	}
	else
	{
		if (m_Connections.count(prt) == 0)
			m_Connections[prt] = ClientAdress;
	}
	Check();
	std::cout << m_Connections.size() << std::endl;
	std::cout << ClientAdress.toString() << std::endl;
}

void Server::Check()
{
	if (m_Connections.size() == 2) m_GameStarted = true;
}

void Server::DeliverPackets()
{
	sf::Packet pack;
	pack << m_GameStarted;
	for (auto& connection : m_Connections)
	{
		if (m_Socket.send(pack, connection.second, connection.first) != sf::Socket::Done)
			std::cout << "Packet haven't been delivered\n";
	}	
}

void Server::Run()
{
	while (true)
	{
		if (m_GameStarted) DeliverPackets();
		else AddConnection();
	}
}
