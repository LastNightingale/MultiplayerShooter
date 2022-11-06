#pragma once
#include <sfml/Graphics.hpp>
#include <vector>
struct ScreenEvent
{
	std::vector<sf::Event> Events;
	sf::Vector2i ScreenPosition;
};