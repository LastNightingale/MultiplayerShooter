#ifndef GAME_MATH_HPP
#define GAME_MATH_HPP

#include "sfml/Graphics.hpp"
#include <cmath>

using namespace sf;

	inline Vector2f VectorCenter(Vector2f sizevector)
	{
		return Vector2f(sizevector.x / 2, sizevector.y / 2);
	}

	inline float VectorLength(Vector2f givenvector)
	{
		return sqrt(pow(givenvector.x, 2) + pow(givenvector.y, 2));
	}

	inline Vector2f NormalizedVector(Vector2f givenvector)
	{
		return Vector2f(givenvector.x / VectorLength(givenvector), givenvector.y / VectorLength(givenvector));
	}

#endif
