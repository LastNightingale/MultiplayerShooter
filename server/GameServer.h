#include <sfml/Graphics.hpp>
#include <sfml/Network.hpp>
#include "PacketOverload.hpp"
#include "GameMath.hpp"
#include "ProgramEvent.hpp"
#include <unordered_map>
#include <unordered_set>
#include "Player.h"
#include <vector>
#include "Connection.hpp"
#include <mutex>
#include <thread>
#include <iostream>
#include "Enemy.h"


class GameServer
{
	Vector2u m_WindowSize = Vector2u(1400, 900);
	std::vector<Entity*> m_Entities;
	std::vector<Entity*> m_DestroyedEntities;	
	RenderList m_CurrentList;
	ProgramEvent m_DrawStarted; //is it needed?
	Player* m_ClientPlayer;
	std::unordered_map<Connection, Player*> m_Players;
	std::unordered_map<Connection, sf::Event> m_Events;
	std::mutex m_EventLock, m_SynchronLock, m_DrawLock, m_PlayerLock;
	Clock m_Clock;
	sf::IpAddress m_ServerIP;
	sf::UdpSocket m_Socket;
	unsigned short m_ServerPort;
	float m_Dt;
	float m_Spawntime;
	int m_CurrentPosition;
	bool m_isRunning;
	bool m_GameStarted;
	bool m_DataDelivered;
	bool m_isSynchronized;
	int m_MaxPlayerAmount;	
	std::unordered_set<Connection> m_Connections;
	bool m_ServerIsRunning;
	sf::RectangleShape m_Rect;
public:
	GameServer();
	void InitPlayer();
	void InitEnemy();
	int IteratePlayer();
	void ServerUpdate();
	void ServerSynchronize();
	void AddConnection();
	void Check();
	void DeliverStartGame();
	void Run();
};