#pragma once
#include <sfml/Graphics.hpp>
#include <unordered_map>

static std::unordered_map<sf::Keyboard::Key, sf::Vector2f> KeyDirections = { { sf::Keyboard::Key::D, {100.f, 0.f} } ,
	{ sf::Keyboard::Key::A, {-100.f, 0.f} } , { sf::Keyboard::Key::W, {0.f, -100.f} } ,
	{ sf::Keyboard::Key::S, {0.f, 100.f} } };
