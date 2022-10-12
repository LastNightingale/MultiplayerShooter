#pragma once
#include <SFML/Network.hpp>
#include <vector>
#include <unordered_map>
#include <unordered_set>

struct Connection
{
	unsigned short Port;
	sf::IpAddress IP;
	Connection(sf::IpAddress ip, unsigned short port)
	{
		Port = port;
		IP = ip;
	}
	Connection(std::string ip, unsigned short port)
	{
		Port = port;
		IP = sf::IpAddress(ip);
	}
	friend bool operator <(const Connection& left, const Connection& right)
	{
		return (left.IP == right.IP) ? (left.Port < right.Port) : (left.IP < right.IP);
	}

	friend bool operator ==(const Connection& left, const Connection& right)
	{
		return (left.IP == right.IP) && (left.Port == right.Port);
	}
};
namespace std
{
	template<>
	struct hash<Connection>
	{
		size_t operator()(const Connection& element) const
		{
			return hash<size_t>()(hash<unsigned short>()(element.Port) + hash<std::string>()(element.IP.toString()));
		}
	};
}

class Server
{
private:
	sf::UdpSocket m_Socket;
	int m_PlayerAmount;
	unsigned short m_Port, m_Port2;
	std::unordered_set<Connection> m_Connections;
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
