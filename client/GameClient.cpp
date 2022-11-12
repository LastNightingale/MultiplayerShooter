#include "GameClient.h"

GameClient::GameClient()
{
	//m_Entities.push_back(new Player());
	//m_ClientPlayer = reinterpret_cast<Player*>(m_Entities[0]);
	m_GameStarted = false; // ������
	m_isRunning = true;
	m_isSynchronized = true;
	m_DataDelivered = false; // ������
	m_ServerPort = 12500;
	m_Socket.setBlocking(true);	
}

void GameClient::Run()
{
	srand(time(NULL));
	while (true)
	{
		sf::Packet pack;
		if (!m_DataDelivered)
		{
			std::cout << "Enter server IP:\n";
			std::cin >> m_ServerIP;
			std::cout << "Enter port:\n";
			std::cin >> m_ClientPort;
			m_Socket.bind(m_ClientPort);
		}
		pack << m_GameStarted;
		if (m_Socket.send(pack, m_ServerIP, m_ServerPort) == sf::Socket::Done)
		{
			m_DataDelivered = true;
			//std::cout << "Delivered\n";
		}
		sf::Packet StartGamePacket;
		sf::IpAddress ServerAddress;
		unsigned short ClientPort;
		if (m_Socket.receive(StartGamePacket, ServerAddress, ClientPort) == sf::Socket::Done)
		{
			StartGamePacket >> m_GameStarted;
			//std::cout << "Packet recieved\n";
		}
		
		if (m_GameStarted)
		{
			thread synchronizethread([this]
				{
					ClientSynchronize();
				});
			thread eventthread([this]
				{
					ClientEvents();
				});
			ClientDraw();
			synchronizethread.join();
			eventthread.join();
		}
	}
}

void GameClient::ClientDraw()
{
	while (m_Window.isOpen())
	{
		//m_DrawStarted.Signal();
		//std::cout << m_GameStarted << std::endl;
		m_EventSignal.Signal();
		sf::Event event;								//later
		std::vector<sf::Event> events;
		while (m_Window.pollEvent(event))
		{
			if(event.type == sf::Event::EventType::MouseButtonPressed)
				events.push_back(event);
		}
		std::cout << "All events :" << m_Events.size() << std::endl;
		m_InteractLock.lock();
		m_Events = events;
		m_InteractLock.unlock();
		//if (m_Events.size() > 0) std::cout << m_Events.size() << std::endl;
		RenderList list;
		m_DrawLock.lock();
		list = m_CurrentList;
		m_DrawLock.unlock();
		//std::cout << "ListToDraw : " << list.Rects.size() << std::endl;
		m_Window.clear(sf::Color::Black);
		for (auto& rect : list.Rects)
		{
			m_Window.draw(rect);
		}
		m_Window.display();		
	}
	m_isRunning = false;
}

void GameClient::ClientSynchronize()
{
	while (m_isSynchronized)
	{
		DeliverData();
		RecieveData();
	}	
}

void GameClient::ClientEvents()
{
	while (m_isRunning)
	{
		
		m_TcpSocket.connect(m_ServerIP, m_ServerPort + (unsigned short)50);
		sf::Vector2i mouseposition = Mouse::getPosition(m_Window);
		sf::Packet DataPacket;
		ScreenEvent scevent;
		std::vector<sf::Event> events;
		m_EventSignal.Wait();
		m_EventLock.lock();
		events = m_Events;
		m_EventLock.unlock();
		std::cout << "Events to deliver :" << events.size() << std::endl;
		scevent.Events = events;
		scevent.ScreenPosition = mouseposition;
		
		DataPacket << static_cast<int>(m_ClientPort) << scevent;
		{
			sf::IpAddress ip;
			int port;
			ScreenEvent scevent;
			DataPacket >> port >> scevent;
			//if(scevent.Events.size() > 0)
			std::cout << port << " " << scevent.Events.size() << std::endl;
		}
		if(m_TcpSocket.send(DataPacket) == sf::Socket::Done) 
		{
		}
		/*m_EventLock.lock();
		m_Events.clear();
		m_EventLock.unlock();*/
	}
}

void GameClient::DeliverData() // deliver events later
{
	//std::cout << m_GameStarted << std::endl;
	
	
	sf::Packet DataPacket;		
	DataPacket << true; // deliver events later
	if (m_Socket.send(DataPacket, m_ServerIP, m_ServerPort) != sf::Socket::Done)
	{
		//std::cout << "Help me" << std::endl;
	}
}

void GameClient::RecieveData()
{
	sf::Packet DataPacket;
	sf::IpAddress ServerAddress;
	unsigned short ClientPort;
	bool temp;
	RenderList list;
	if (m_Socket.receive(DataPacket, ServerAddress, ClientPort) == sf::Socket::Done)
	{
		DataPacket >> temp >> list;
		m_SynchronLock.lock();
		m_CurrentList = list;
		m_SynchronLock.unlock();
		//std::cout << "List size: " << list.Rects.size() << " Packet size: " << DataPacket.getDataSize() << std::endl;
	}
}
