#pragma once 
#include "SFML/Graphics.hpp"
#include <cmath>

using namespace sf;

Vector2f VectorCenter(Vector2f sizevector)
{
	return Vector2f(sizevector.x / 2, sizevector.y / 2);
}

float VectorLength(Vector2f givenvector)
{
	return sqrt(pow(givenvector.x, 2) + pow(givenvector.y, 2));
}

Vector2f NormalizedVector(Vector2f givenvector)
{
	float length = VectorLength(givenvector);
	return Vector2f(givenvector.x / length, givenvector.y / length)
}