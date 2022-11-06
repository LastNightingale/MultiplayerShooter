#pragma once
#include <sfml/Graphics.hpp>
#include <sfml/Network.hpp>
#include "GameMath.hpp"
#include "RenderList.hpp"
#include "ScreenEvent.hpp"
#include <iostream>
namespace sf
{
	inline Packet& operator <<(Packet& packet, const Color& color) //color write
	{
		return packet << color.r << color.g << color.b << color.a;
	}
	inline Packet& operator >>(Packet& packet, Color& color) //color recieve
	{
		return packet >> color.r >> color.g >> color.b >> color.a;
	}

	inline Packet& operator <<(Packet& packet, const Vector2f& vector) //vector write
	{
		return packet << vector.x << vector.y;
	}
	inline Packet& operator >>(Packet& packet, Vector2f& vector) //vector recieve
	{
		return packet >> vector.x >> vector.y;
	}

	inline Packet& operator <<(Packet& packet, const RectangleShape& rect)
	{
		sf::Vector2f size, position;
		sf::Color color;
		size = rect.getSize();
		position = rect.getPosition();
		color = rect.getFillColor();
		return packet << size << position << color;
	}
	inline Packet& operator >>(Packet& packet, RectangleShape& rect)
	{
		sf::Vector2f size, position;
		sf::Color color;
		packet >> size >> position >> color;
		rect.setSize(size);
		rect.setOrigin(VectorCenter(size));
		rect.setPosition(position);
		rect.setFillColor(color);		
		return packet;
	}

	template <typename T>
	inline Packet& operator <<(Packet& packet, const std::vector<T>& values)
	{
		int size = values.size();
		packet << size;
		for (int i = 0; i < size; ++i)
			packet << values[i];
		return packet;
	}
	template <typename T>
	inline Packet& operator >>(Packet& packet, std::vector<T>& values)
	{
		int size;
		T value;
		packet >> size;
		for (int i = 0; i < size; ++i)
		{
			packet >> value;
			values.push_back(value);
		}
		return packet;
	}

	inline Packet& operator <<(Packet& packet, const RenderList& list)
	{		
		//std::cout << "To Deliver List: " << list.Rects.size() << std::endl;
		return packet << list.Rects;
	}
	inline Packet& operator >>(Packet& packet, RenderList& list)
	{		
		//std::cout << "To Recieve List: " << list.Rects.size() << std::endl;
		return packet >> list.Rects;
	}
	//overload for sf::Event

	//SizeEvent
	inline Packet& operator <<(Packet& packet, const Event::SizeEvent& sizeevent)
	{
		return packet << sizeevent.height << sizeevent.width;
	}
	inline Packet& operator >>(Packet& packet, Event::SizeEvent& sizeevent)
	{
		return packet >> sizeevent.height >> sizeevent.width;
	}

	//KeyEvent
	inline Packet& operator <<(Packet& packet, const Keyboard::Key& key)
	{
		return packet << static_cast<int>(key);
	}
	inline Packet& operator >>(Packet& packet, Keyboard::Key& key)
	{
		int k;
		packet >> k;
		key = static_cast<Keyboard::Key>(k);
		return packet;
	}

	//KeyEvent
	inline Packet& operator <<(Packet& packet, const Event::KeyEvent& keyevent)
	{
		return packet << keyevent.alt << keyevent.code << keyevent.control << keyevent.shift << keyevent.system;
	}
	inline Packet& operator >>(Packet& packet, Event::KeyEvent& keyevent)
	{
		return packet >> keyevent.alt >> keyevent.code >> keyevent.control >> keyevent.shift >> keyevent.system;
	}

	//EventType
	inline Packet& operator <<(Packet& packet, const Event::EventType& eventtype)
	{
		return packet << static_cast<int>(eventtype);
	}
	inline Packet& operator >>(Packet& packet, Event::EventType& eventtype)
	{
		int et;
		packet >> et;
		eventtype = static_cast<Event::EventType>(et);
		return packet;
	}

	//Event
	inline Packet& operator <<(Packet& packet, const Event& event)
	{
		return packet << event.type << event.key;
	}
	inline Packet& operator >>(Packet& packet, Event& event)
	{
		return packet >> event.type >> event.key;
	}

	//Vector2i
	inline Packet& operator <<(Packet& packet, const Vector2i& vector) 
	{
		return packet << vector.x << vector.y;
	}
	inline Packet& operator >>(Packet& packet, Vector2i& vector) 
	{
		return packet >> vector.x >> vector.y;
	}

	//ScreenEvent
	inline Packet& operator <<(Packet& packet, const ScreenEvent& scevent)
	{
		return packet << scevent.Events << scevent.ScreenPosition;
	}
	inline Packet& operator >>(Packet& packet, ScreenEvent& scevent)
	{
		return packet >> scevent.Events >> scevent.ScreenPosition;
	}
}
