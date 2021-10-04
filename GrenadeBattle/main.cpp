#include <SFML/Graphics.hpp>
#include <Math.h>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

int main()
{
	sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Grenade Battle", sf::Style::Titlebar | sf::Style::Close);
	sf::Texture playerTexture;
	playerTexture.loadFromFile("Assets/Sprites/boy.png");

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
	float jumpForce = 100.0f;
	float playerSpeed = 3000.0f;
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

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			playerAcceleration.x = -ACCEL_RATE;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			playerAcceleration.x = ACCEL_RATE;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			playerAcceleration.y = -jumpForce;
		}

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}
		}
		//UPDATE
		sf::Vector2f deltaVelocity = playerAcceleration * deltaTime;
		playerVelocity += deltaVelocity;
		playerVelocity.x = playerVelocity.x * drag;

		sf::Vector2f deltaPosition = playerVelocity * deltaTime;

		playerPosition += deltaPosition;

		if (playerPosition.y >= 500.0f)
		{
			playerPosition.y = 500.0f;
		}

		playerSprite.setPosition(playerPosition);

		window.clear();
		window.draw(playerSprite);
		window.display();
	}

	return 0;
}