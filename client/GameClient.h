#pragma once
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
#include "PacketOverload.hpp"
#include "ProgramEvent.hpp"
#include <SFML/Network.hpp>
#include <sfml/Graphics.hpp>

using namespace std;

//using namespace sf;

class GameClient
{
private:
	sf::RenderWindow m_Window{ sf::VideoMode({ 1400,900 }) , "GameClient", sf::Style::Close };
	RenderList m_CurrentList;
	ProgramEvent m_DrawStarted;
	mutex m_DrawLock, m_EventLock, m_SynchronLock;
	sf::IpAddress m_ServerIP;
	sf::UdpSocket m_Socket;
	vector<sf::Event> m_Events;
	unsigned short m_ServerPort, m_ClientPort;
	bool m_isRunning;
	bool m_GameStarted;
	bool m_DataDelivered;
	bool m_isSynchronized;
public:
	GameClient();
	void Run();
private:
	//void ClientUpdate();
	void ClientDraw();
	void ClientSynchronize();
	void DeliverData();
	void RecieveData();
};

