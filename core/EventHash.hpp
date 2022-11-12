#pragma once
#include <sfml/Graphics.hpp>
namespace sf
{
	class TcpEvent : public Event
	{
		friend bool operator <(const TcpEvent& left, const TcpEvent& right)
		{
			return (left.key.code == right.key.code) ? (left.key.code < right.key.code) : (left.type < right.type);
		}

		friend bool operator ==(const TcpEvent& left, const TcpEvent& right)
		{
			return (left.key.code == right.key.code) && (left.type == right.type);
		}
	};
}


namespace std
{
	template<>
	struct hash<sf::TcpEvent>
	{
		size_t operator()(const sf::Event& event) const
		{
			return hash<size_t>()(hash<sf::Keyboard::Key>()(event.key.code) + hash<sf::Event::EventType>()(event.type));
		}
	};
}