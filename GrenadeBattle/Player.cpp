#include "Player.h"

Player::Player()
{

}

Player::Player(sf::Sprite _playerSprite, sf::Vector2f _playerPos, sf::Vector2f _playerVelocity, sf::Vector2f _playerBoundsOffset, sf::Vector2f _playerBoundsScale, sf::Vector2f _playerCenter)
	: playerSprite(_playerSprite)
	, playerPosition(_playerPos)
	, playerBoundsOffset(_playerBoundsOffset)
	, playerBoundsScale(_playerBoundsScale)
	, playerCenter(_playerCenter)

	, playerAcceleration(sf::Vector2f(0.0f, 0.0f))
	, terminalVelocity(sf::Vector2f(0.0f, 15.0f))
	, playerVelocity(sf::Vector2f(0.0f, 0.0f))
	, playerRect(playerSprite.getGlobalBounds())
	, playerRectSize(playerRect.width, playerRect.height)
{

}
