#include <SFML/Graphics.hpp>
#include <Math.h>
#include <Player.h>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

void PlayerOneInput(sf::Vector2f & _playerAcceleration, float _ACCEL_RATE, float & _jumpForce);
void PlayerTwoInput(sf::Vector2f & _playerTwoAcceleration, float _ACCEL_RATE, float & _jumpForce);
void PlayerMovement(sf::Vector2f & _playerVelocity, sf::Vector2f & _playerAcceleration, float _deltaTime, sf::Vector2f & _playerPosition, float _drag);
bool IntersectCheck(sf::Vector2f _spherePos, sf::Vector2f _otherPos, float _radius);
void Intersect(sf::FloatRect _playerRect, sf::FloatRect _playerTwoRect, sf::Vector2f _playerCenter, sf::Vector2f _playerTwoCenter, sf::Vector2f _colDepth, sf::Vector2f _playerPos, sf::Vector2f _playerVelocity);

int main()
{
	sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Grenade Battle", sf::Style::Titlebar | sf::Style::Close);
	sf::Texture playerTexture;
	playerTexture.loadFromFile("Assets/Sprites/boy.png");

/* Player Code
	//---- PLAYER ONE CODE ----
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

	//Rects
	sf::FloatRect playerRect = playerSprite.getGlobalBounds();
	sf::Vector2f playerRectSize = sf::Vector2f(playerRect.width, playerRect.height);

	//Bounding boxes
	sf::Vector2f playerBoundsOffset(3.0f, 0.0f);
	sf::Vector2f playerBoundsScale(0.75f, 1.0f);

	//Center of player
		//Setup player center
	sf::Vector2f playerCenter = sf::Vector2f(0.0f, 0.0f);


	//---- END OF PLAYER ONE CODE ----

	//---- PLAYER TWO CODE ----
	sf::Sprite playerTwoSprite;
	playerTwoSprite.setTexture(playerTexture);
	playerTwoSprite.setScale(.1f, .1f);
	sf::Vector2f playerTwoPosition = sf::Vector2f(300.0f, 200.0f);
	playerTwoSprite.setPosition(playerTwoPosition);

	//Velocity
	sf::Vector2f playerTwoVelocity = sf::Vector2f(200.0f, 0.0f);

	//Gravity times 2 is being applied in playerAcceleration.
	sf::Vector2f playerTwoAcceleration = sf::Vector2f(0.0f, 0.0f);

	//Rect
	sf::FloatRect playerTwoRect = playerSprite.getGlobalBounds();
	sf::Vector2f playerTwoRectSize = sf::Vector2f(playerTwoRect.width, playerTwoRect.height);
	
	//Bounding Boxes
	sf::Vector2f playerTwoBoundsOffset(-7.0f, -7.0f);
	sf::Vector2f playerTwoBoundsScale(1.5f, 1.5f);

	//Center of player two
	//Setup player center
	sf::Vector2f playerTwoCenter = sf::Vector2f(0.0f, 0.0f);

	//---- END OF PLAYER TWO CODE ----
*/

	Player playerOne();

	float jumpForce = 100.0f;
	float playerSpeed = 3000.0f;
	//Constant
	const float ACCEL_RATE = 7000.0f;
	float drag = .99f;

	//Collision
	sf::Vector2f colDepth = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f previousPos = sf::Vector2f(0.0f, 0.0f);

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
		PlayerMovement(playerVelocity, playerAcceleration, deltaTime, playerPosition, drag);
		PlayerMovement(playerTwoVelocity, playerTwoAcceleration, deltaTime, playerTwoPosition, drag);

		if (playerPosition.y >= 500.0f)
		{
			playerPosition.y = 500.0f;
		}

		if (playerTwoPosition.y >= 500.0f)
		{
			playerTwoPosition.y = 500.0f;
		}

		playerSprite.setPosition(playerPosition);
		playerTwoSprite.setPosition(playerTwoPosition);

		if (playerRect.intersects(playerTwoRect))
		{
			Intersect(playerRect, playerTwoRect, playerCenter, playerTwoCenter, colDepth, playerPosition, playerVelocity);
		}


		window.clear();
		window.draw(playerSprite);
		window.draw(playerTwoSprite);
		window.display();
	}

	return 0;
}

void PlayerOneInput(sf::Vector2f & _playerAcceleration, float _ACCEL_RATE, float & _jumpForce)
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

void PlayerTwoInput(sf::Vector2f & _playerTwoAcceleration, float _ACCEL_RATE, float & _jumpForce)
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

void PlayerMovement(sf::Vector2f & _playerVelocity, sf::Vector2f & _playerAcceleration, float _deltaTime, sf::Vector2f & _playerPosition, float _drag)
{
	sf::Vector2f deltaVelocity = _playerAcceleration * _deltaTime;
	_playerVelocity += deltaVelocity;
	_playerVelocity.x = _playerVelocity.x * _drag;

	sf::Vector2f deltaPosition = _playerVelocity * _deltaTime;

	_playerPosition += deltaPosition;
}

bool IntersectCheck(sf::Vector2f _spherePos, sf::Vector2f _otherPos, float _radius)
{
	float distance = sqrt((_spherePos.x - _otherPos.x) * (_spherePos.x - _otherPos.x) +
		(_spherePos.y - _otherPos.y) * (_spherePos.y - _otherPos.y));

	return (distance < (_radius + _radius));
}

void Intersect(sf::FloatRect _playerRect, sf::FloatRect _playerTwoRect, sf::Vector2f _playerCenter, sf::Vector2f _playerTwoCenter, sf::Vector2f _colDepth, sf::Vector2f _playerPos, sf::Vector2f _playerVelocity)
{
	//Check collision depth
	sf::Vector2f playerMinCenterDist = sf::Vector2f(_playerRect.width / 2 + _playerTwoRect.width / 2,
		_playerRect.height / 2 + _playerTwoRect.height / 2);

	sf::Vector2f distBtwnCenters = _playerTwoCenter - _playerCenter;

	_colDepth = (distBtwnCenters - playerMinCenterDist);


	//Change player pos to position before colliding
	//playerPos = previousPos;

	if (playerMinCenterDist.x < 0)
	{
		playerMinCenterDist.x = -playerMinCenterDist.x;
	}
	if (playerMinCenterDist.y < 0)
	{
		playerMinCenterDist.y = -playerMinCenterDist.y;
	}

	_colDepth = playerMinCenterDist - distBtwnCenters;


	if (std::abs(_colDepth.y) < std::abs(_colDepth.x))
	{
		_playerPos.y -= _colDepth.y;
		_playerVelocity.y = 0;
	}
	else
	{
		_playerPos.x -= _colDepth.x;
		_playerVelocity.x = 0;
	}
	_playerVelocity = sf::Vector2f(0.0f, 0.0f);
}