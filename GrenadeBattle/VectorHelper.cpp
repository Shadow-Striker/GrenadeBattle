#include <SFML/Graphics.hpp>
#include <math.h>
#include "VectorHelper.h"
float VectorMagnitude(sf::Vector2f _vector)
{
	//Calculations
	float mag = sqrt(_vector.x * _vector.x + _vector.y * _vector.y);

	//Return answer
	return mag;
}

float VectorMagnitude(sf::Vector3f _vector)
{
	//Calculations
	float mag = sqrt(_vector.x * _vector.x + _vector.y * _vector.y + _vector.z * _vector.z);

	//Return answer
	return mag;
}

sf::Vector2f VectorNormalize(sf::Vector2f _vector)
{
	return _vector / VectorMagnitude(_vector);
}

sf::Vector3f VectorNormalize(sf::Vector3f _vector)
{
	return 	_vector / VectorMagnitude(_vector);
}

sf::Vector3f VectorCross(sf::Vector3f a, sf::Vector3f b)
{
	sf::Vector3f c;
	c.x = a.y * b.z - a.z * b.y;
	c.y = a.z * b.x - a.x * b.z;
	c.z = a.x * b.y - a.y * b.x;
	return c;
}

sf::Vector2f CollisionDepth(sf::FloatRect a, sf::FloatRect b)
{
	//Width and height of each rectangle
	sf::Vector2f aWidthHeight = sf::Vector2f(a.width, a.height);
	sf::Vector2f bWidthHeight = sf::Vector2f(b.width, b.height);

	//Start from the top left corner, get to the centre point.
	sf::Vector2f aCentre = sf::Vector2f(a.left, a.top) + 0.5f * aWidthHeight;
	sf::Vector2f bCentre = sf::Vector2f(b.left, b.top) + 0.5f * bWidthHeight;

	sf::Vector2f distance = aCentre - bCentre;
	sf::Vector2f minDistance = 0.5f * aWidthHeight + 0.5f * bWidthHeight;

	//Account for direction of collision
	if (distance.x < 0)
	{
		minDistance.x = -minDistance.x;
	}
	if (distance.y < 0)
	{
		minDistance.y = -minDistance.y;
	}

	sf::Vector2f depth = minDistance = distance;

	return depth;
}

float VectorDot(sf::Vector2f a, sf::Vector2f b)
{
	return 	a.x * b.x + a.y * b.y;
}

float VectorDot(sf::Vector3f a, sf::Vector3f b)
{
	return 	a.x * b.x + a.y * b.y + a.z * b.z;
}

