#pragma once
#include <SFML/Network.hpp>
//#include "../Enemy.h"
//#include "../Player.h"
#include <vector>

class Server
{
private:
	sf::UdpSocket m_Socket;
	int m_PlayerAmount;
	unsigned short m_Port, m_Port2;
	std::vector<sf::IpAddress> m_Connections;
	//std::vector<Entity*> m_Entities;
	bool m_GameStarted;
public:
	Server();
	void AddConnection();
	void Check();
	void DeliverPackets();
	void RecievePackets() {};
	void Run();
};
