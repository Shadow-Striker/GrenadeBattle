#pragma once
#include <SFML/Graphics.hpp>
#include <Math.h>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

class Player
{
public:

	Player();
	Player(sf::Sprite _playerSprite, sf::Vector2f _playerPos, sf::Vector2f _playerVelocity, sf::Vector2f _playerBoundsOffScale, sf::Vector2f _playerBoundsScale, sf::Vector2f _playerCenter);
	
	sf::Sprite playerSprite;
	sf::Vector2f playerPosition;

	//Velocity
	sf::Vector2f playerVelocity;
	sf::Vector2f terminalVelocity;

	//Gravity times 2 is being applied in playerAcceleration.
	sf::Vector2f playerAcceleration;

	//Rect
	sf::FloatRect playerRect = playerSprite.getGlobalBounds();
	sf::Vector2f playerRectSize = sf::Vector2f(playerRect.width, playerRect.height);

	//Bounding boxes
	sf::Vector2f playerBoundsOffset;
	sf::Vector2f playerBoundsScale;

	//Center of player
	sf::Vector2f playerCenter = sf::Vector2f(0.0f, 0.0f);

};