#pragma once
#include <vector>
#include "Enemy.h"
#include "Player.h"
#include "Bullet.h"
#include <thread>
#include <mutex>
#include <Windows.h>
#include "GameMath.hpp"
#include <SFML/Network.hpp>

using namespace std;

class ProgramEvent {
private:
	HANDLE m_Handle;
public:
	ProgramEvent() {
		m_Handle = CreateEvent(
			NULL,               // default security attributes
			FALSE,               // manual-reset event
			FALSE,              // initial state is nonsignaled
			TEXT("WriteEvent")  // object name
		);
	}

	~ProgramEvent() {
		CloseHandle(m_Handle);
	}

	void Signal() {
		SetEvent(m_Handle);
	}

	void Wait() {
		WaitForSingleObject(m_Handle, INFINITE);
	}
};

class Game
{
private:
	RenderWindow m_Window{ VideoMode({ 1400,900 }) , "Shooter", sf::Style::Close};
	vector<Entity*> m_Entities;
	vector<Entity*> m_DestroyedEntities;
	RenderList m_CurrentList;
	ProgramEvent m_DrawStarted;
	Player* m_ClientPlayer;
	mutex m_Lock;
	Clock m_Clock;
	sf::UdpSocket m_Socket;
	unsigned short m_Port, m_Port2;
	float m_Dt;
	float m_Spawntime;
	bool m_isRunning;
	bool m_GameStarted = false;
	bool m_DataDelivered;

public:
	Game();
	void SpawnEnemy();
	void GameUpdate(float dt);
	void Collision();
	void GameDraw();
	void Run();
};

