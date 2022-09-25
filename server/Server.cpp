#include "Server.h"
#include <iostream>

Server::Server()
{
	m_PlayerAmount = 0;
	m_Port = 12500;
	m_Port2 = 12501;
	m_GameStarted = false;
	//m_Socket.bind(m_Port);
	m_Socket.setBlocking(false);
}

void Server::AddConnection()
{
	sf::Packet packet;
	unsigned short prt;
	sf::IpAddress ClientAdress(127, 0, 0, 1);
	if (m_Socket.receive(packet, ClientAdress, prt) != sf::Socket::Done)
	{
		std::cout << "No new connections\n";
	}
	else
	{
		m_Connections.push_back(ClientAdress);
		m_PlayerAmount++;
	}
	Check();
	std::cout << m_Connections.size() << std::endl;
	std::cout << ClientAdress.toString() << std::endl;
}

void Server::Check()
{
	if (m_Connections.size() == 1) m_GameStarted = true;
}

void Server::DeliverPackets()
{
	sf::Packet pack;
	pack << m_GameStarted;
	for (sf::IpAddress adress : m_Connections)
	{
		if (m_Socket.send(pack, adress, m_Port) != sf::Socket::Done)
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
