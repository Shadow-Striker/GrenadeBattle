#include <SFML/Graphics.hpp>
#include <Math.h>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

void PlayerOneInput(sf::Vector2f & _playerAcceleration, float _ACCEL_RATE, float _jumpForce);
void PlayerTwoInput(sf::Vector2f & _playerTwoAcceleration, float _ACCEL_RATE, float _jumpForce);
void PlayerOneMovement(sf::Vector2f _playerVelocity, sf::Vector2f &_playerAcceleration, float _deltaTime, sf::Vector2f _playerPosition, float _drag);

int main()
{
	sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Grenade Battle", sf::Style::Titlebar | sf::Style::Close);
	sf::Texture playerTexture;
	playerTexture.loadFromFile("Assets/Sprites/boy.png");

	//Player One Code
	sf::Sprite playerSprite;
	playerSprite.setTexture(playerTexture);
	playerSprite.setScale(.1f, .1f);	
	sf::Vector2f playerPosition = sf::Vector2f(300.0f, 200.0f);
	playerSprite.setPosition(playerPosition);
	//Velocity
	sf::Vector2f playerVelocity = sf::Vector2f(200.0f, 0.0f);
	sf::Vector2f terminalVelocity = sf::Vector2f(0.0f, 15.0f);
	//Gravity times 2 is being applied in playerAcceleration.
	sf::Vector2f playerAcceleration = sf::Vector2f(0.0f, 0.0f);

	//Player Two Code
	sf::Sprite playerTwoSprite;
	playerTwoSprite.setTexture(playerTexture);
	playerTwoSprite.setScale(.1f, .1f);
	sf::Vector2f playerTwoPosition = sf::Vector2f(300.0f, 200.0f);
	playerTwoSprite.setPosition(playerTwoPosition);
	sf::Vector2f playerTwoAcceleration = sf::Vector2f(0.0f, 0.0f);

	float jumpForce = 100.0f;
	float playerSpeed = 3000.0f;
	//Constant
	const float ACCEL_RATE = 7000.0f;
	float drag = .99f;

	sf::Clock gameClock; 

	while (window.isOpen())
	{
		sf::Time frameTime = gameClock.restart();
		float deltaTime = frameTime.asSeconds();

		//INPUT
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		playerAcceleration.x = 0.0f;
		playerAcceleration.y = 15.0f;

		playerTwoAcceleration.x = 0.0f;
		playerTwoAcceleration.y = 15.0f;

		PlayerOneInput(playerAcceleration, ACCEL_RATE, jumpForce);
		PlayerTwoInput(playerTwoAcceleration, ACCEL_RATE, jumpForce);

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}
		}
		//UPDATE
		//Velocity code
		PlayerOneMovement(playerVelocity, playerAcceleration, deltaTime, playerPosition, drag);

		if (playerPosition.y >= 500.0f)
		{
			playerPosition.y = 500.0f;
		}

		playerSprite.setPosition(playerPosition);

		window.clear();
		window.draw(playerSprite);
		window.draw(playerTwoSprite);
		window.display();
	}

	return 0;
}

void PlayerOneInput(sf::Vector2f &_playerAcceleration, float _ACCEL_RATE, float _jumpForce)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		_playerAcceleration.x = -_ACCEL_RATE;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		_playerAcceleration.x = _ACCEL_RATE;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		_playerAcceleration.y = -_jumpForce;
	}
}

void PlayerTwoInput(sf::Vector2f &_playerTwoAcceleration, float _ACCEL_RATE, float _jumpForce)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		_playerTwoAcceleration.x = -_ACCEL_RATE;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		_playerTwoAcceleration.x = _ACCEL_RATE;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
	{
		_playerTwoAcceleration.y = -_jumpForce;
	}
}

void PlayerOneMovement(sf::Vector2f _playerVelocity, sf::Vector2f &_playerAcceleration, float _deltaTime, sf::Vector2f _playerPosition, float _drag)
{
	sf::Vector2f deltaVelocity = _playerAcceleration * _deltaTime;
	_playerVelocity += deltaVelocity;
	_playerVelocity.x = _playerVelocity.x * _drag;

	sf::Vector2f deltaPosition = _playerVelocity * _deltaTime;

	_playerPosition += deltaPosition;
}