#pragma once
#include <SFML/Graphics.hpp>

float VectorMagnitude(sf::Vector2f _vector);
float VectorMagnitude(sf::Vector3f _vector);

float VectorDot(sf::Vector2f a, sf::Vector2f b);
float VectorDot(sf::Vector3f a, sf::Vector3f b);

sf::Vector2f VectorNormalize(sf::Vector2f _vector);
sf::Vector3f VectorNormalize(sf::Vector3f _vector);

sf::Vector3f VectorCross(sf::Vector3f a, sf::Vector3f b);

sf::Vector2f CollisionDepth(sf::FloatRect a, sf::FloatRect b);