#include <SFML/Graphics.hpp>
#include <Math.h>
#include "VectorHelper.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

void PlayerOneInput(sf::Vector2f & _playerAcceleration, float _ACCEL_RATE, float & _jumpForce);
void PlayerTwoInput(sf::Vector2f & _playerTwoAcceleration, float _ACCEL_RATE, float & _jumpForce);
void PlayerMovement(sf::Vector2f & _playerVelocity, sf::Vector2f & _playerAcceleration, float & _deltaTime, sf::Vector2f & _playerPosition, float & _drag);
bool IntersectCheck(sf::Vector2f _spherePos, sf::Vector2f _otherPos, float _radius);
void Intersect(sf::FloatRect & _playerRect, sf::FloatRect & _playerTwoRect, sf::Vector2f & _playerCenter, sf::Vector2f & _playerTwoCenter, sf::Vector2f & _colDepth, sf::Vector2f & _playerPos, sf::Vector2f & _playerVelocity, sf::RectangleShape & _playerRectDisplay, sf::RectangleShape & _playerTwoRectDisplay);
void QuadEaseOut(sf::Vector2f & _change, float & _time, float & _duration, float & _deltaTime, sf::Vector2f & _begin, sf::Vector2f& _end, sf::Vector2f & _uiPosition);


int main()
{
	sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "Grenade Battle", sf::Style::Titlebar | sf::Style::Close);
	sf::Texture playerTexture;
	playerTexture.loadFromFile("Assets/Sprites/boy.png");

	//SETUP OF PIPS

	std::vector<sf::Sprite> pips;
	sf::Texture pipTexture;
	pipTexture.loadFromFile("Assets/Sprites/pip.png");
	const int NUM_PIPS = 10;
	for (size_t i = 0; i < NUM_PIPS; ++i)
	{
		sf::Sprite newPip;
		newPip.setTexture(pipTexture);
		newPip.setScale(1.0f, 1.0f);
		newPip.setOrigin(pipTexture.getSize().x / 2, pipTexture.getSize().y / 2);
		pips.push_back(newPip);
	}

	//Grenade
	sf::Texture grenadeTexture;
	grenadeTexture.loadFromFile("Assets/Sprites/grenade.png");
	sf::Sprite grenadeSprite;
	grenadeSprite.setTexture(grenadeTexture);
	sf::Vector2f grenadeVelocity(0.0f, 0.0f);

	//Crate
	sf::Texture crateTexture;
	crateTexture.loadFromFile("Assets/Sprites/crate.png");
	const int NUM_CRATES = 30;
	std::vector<sf::Sprite> crateSprites;
	for (size_t i = 0; i < NUM_CRATES; i++)
	{
		crateSprites.push_back(sf::Sprite());
		crateSprites[i].setTexture(crateTexture);
		crateSprites[i].setPosition(i * 55, 500);
	}
	sf::Vector2f crateSpriteSize;
	crateSpriteSize.x = crateTexture.getSize().x;
	crateSpriteSize.y = crateTexture.getSize().y;

	//Practical Task 3 - Gravity Prediction
	sf::Vector2f initialVelocity(500, -1000);
	sf::Vector2f initialPosition;
	sf::Vector2f gravityVector(0, 1000.0f);
	sf::Vector2f firingDirection(1.0f, 0);
	sf::Vector2f firingPosition(500, 500);
	sf::Vector2f firingVelocity(5000.0f, -500.0f);
	float firingSpeed = 750.0f;

	//INTERPOLATION
	sf::Sprite uiSprite;
	sf::Texture uiTexture;
	uiTexture.loadFromFile("Assets/Sprites/UI Panel.png");
	uiSprite.setTexture(uiTexture);
	//uiSprite.setScale(.1f, .1f);
	uiSprite.setOrigin(uiTexture.getSize().x / 2, uiTexture.getSize().y / 2);
	sf::Vector2f uiPosition(0.0f, 0.0f);
	uiSprite.setPosition(uiPosition);


	sf::Vector2f end(sf::VideoMode::getDesktopMode().width / 2.0f, sf::VideoMode::getDesktopMode().height / 2.0f);
	sf::Vector2f begin(sf::VideoMode::getDesktopMode().width / 2.0, sf::VideoMode::getDesktopMode().height);
	sf::Vector2f change = end - begin;

	float duration = 1.0f;
	float time = 0.0f;

	//---- PLAYER ONE CODE ----
	sf::Sprite playerSprite;
	playerSprite.setTexture(playerTexture);
	playerSprite.setScale(.1f, .1f);
	sf::Vector2f playerPosition = sf::Vector2f(300.0f, 200.0f);
	playerSprite.setPosition(playerPosition);

	//Velocity
	sf::Vector2f playerVelocity = sf::Vector2f(0.0f, 0.0f);
	sf::Vector2f terminalVelocity = sf::Vector2f(0.0f, 70.0f);

	//Gravity
	sf::Vector2f gravity = sf::Vector2f(0.0f, 750.0f);

	//Gravity times 2 is being applied in playerAcceleration.
	sf::Vector2f playerAcceleration = sf::Vector2f(0.0f, 0.0f);

	//Bounding boxes
	sf::Vector2f playerBoundsOffset(0.0f, 0.0f);
	sf::Vector2f playerBoundsScale(1.0f, 1.0f);

	sf::Vector2f spriteSize;
	spriteSize.x = playerTexture.getSize().x;
	spriteSize.y = playerTexture.getSize().y;

	// Bounding circles
	/*float playerCircleRadius = playerTexture.getSize().x / 2;
	sf::Vector2f playerCircleCentre = playerPosition + 0.5f * spriteSize;
	sf::CircleShape playerCircle;
	playerCircle.setRadius(playerCircleRadius);
	playerCircle.setPosition(playerPosition);
	playerCircle.setFillColor(sf::Color::Green);*/

	//Visual
	sf::RectangleShape playerRectDisplay;

	//Center of player
		//Setup player center
	sf::Vector2f playerCenter = playerPosition + 0.5f * spriteSize;


	//---- END OF PLAYER ONE CODE ----

	//---- PLAYER TWO CODE ----
	sf::Sprite playerTwoSprite;
	playerTwoSprite.setTexture(playerTexture);
	playerTwoSprite.setScale(.1f, .1f);
	sf::Vector2f playerTwoPosition = sf::Vector2f(500.0f, 200.0f);
	playerTwoSprite.setPosition(playerTwoPosition);

	//Velocity
	sf::Vector2f playerTwoVelocity = sf::Vector2f(200.0f, 0.0f);

	//Gravity times 2 is being applied in playerAcceleration.
	sf::Vector2f playerTwoAcceleration = sf::Vector2f(0.0f, 0.0f);

	//Bounding Boxes
	sf::Vector2f playerTwoBoundsOffset(0.0f, 0.0f);
	sf::Vector2f playerTwoBoundsScale(1.0f, 1.0f);


	//Bounding Circle
	/*float playerTwoCircleRadius = playerTexture.getSize().x / 2;
	sf::Vector2f playerTwoCircleCentre = playerTwoPosition + 0.5f * spriteSize;
	sf::CircleShape playerTwoCircle;
	playerTwoCircle.setRadius(playerTwoCircleRadius);
	playerTwoCircle.setPosition(playerTwoPosition);
	playerTwoCircle.setFillColor(sf::Color::Green);*/

	//Visual
	sf::RectangleShape playerTwoRectDisplay;

	//Center of player two
	//Setup player center
	sf::Vector2f playerTwoCenter = playerTwoPosition + 0.5f * spriteSize;

	//---- END OF PLAYER TWO CODE ----

	float jumpForce = 300.0f;
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
		//INPUT
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		playerAcceleration.x = 0.0f;
		playerAcceleration.y = 120.0f;
		playerTwoAcceleration.x = 0.0f;
		playerTwoAcceleration.y = 120.0f;

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}
		}
		//Game Clock
		sf::Time frameTime = gameClock.restart();
		float deltaTime = frameTime.asSeconds();
		QuadEaseOut(change, time, duration, deltaTime, begin, end, uiPosition);
		uiSprite.setPosition(uiPosition);



		sf::Vector2f lastAcceleration = playerAcceleration;
		sf::Vector2f lastVelocity = playerVelocity;
		playerAcceleration.x = 0;
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			grenadeSprite.setPosition(firingPosition);
			grenadeVelocity = firingVelocity;
		}

		//UPDATE 
		firingPosition = playerSprite.getPosition();
		firingVelocity = sf::Vector2f(sf::Mouse::getPosition(window)) - firingPosition;

		// Normalize the velocity
		float mag = sqrt(firingVelocity.x * firingVelocity.x + firingVelocity.y * firingVelocity.y);
		firingVelocity /= mag;

		// Scale velocity by speed
		firingVelocity *= firingSpeed;
		
		playerTwoAcceleration.x = 0;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			playerTwoAcceleration.x = -ACCEL_RATE;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			playerTwoAcceleration.x = ACCEL_RATE;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
		{
			playerTwoAcceleration.y = -jumpForce;
		}

		//Practical Task 4 - Physics Alternatives (PLEASE COMMENT OUT EACH METHOD)
		//---EXPLICIT EULER--- (Uses previous frame's values in current frame's calculations
		/*playerVelocity += lastAcceleration * deltaTime;
		playerVelocity = 0.9f * playerVelocity;
		playerPosition += lastVelocity * deltaTime;*/
		//END OF EXPLICIT EULER

		//IMPLICIT EULER
		/*playerVelocity += playerAcceleration * deltaTime;
		playerVelocity = 0.9f * playerVelocity;
		playerPosition += playerVelocity * deltaTime;*/
		//END OF IMPLICIT EULER

		//SEMI IMPLICIT EULER
		/*playerVelocity += lastAcceleration * deltaTime;
		playerVelocity = 0.9f * playerVelocity;
		playerPosition += playerVelocity * deltaTime;*/
		//END OF SEMI IMPLICIT EULER

		//VELOCITY VERLET
		/*sf::Vector2f firstHalfVel = lastVelocity + lastAcceleration * (0.5f * deltaTime);
		firstHalfVel = 0.9f * firstHalfVel; // drag
		playerPosition += firstHalfVel * deltaTime;
		playerVelocity = firstHalfVel + playerAcceleration * (deltaTime * 0.5f);*/
		//END OF VELOCITY VERLET

		if (playerPosition.y >= 500.0f) playerPosition.y = 500.0f;
		if (playerTwoPosition.y >= 500.0f) playerTwoPosition.y = 500.0f;


		sf::Vector2f deltaPosition = playerVelocity * deltaTime;
		playerPosition += deltaPosition;
		sf::Vector2f deltaVelocity = playerAcceleration * deltaTime;
		playerVelocity += deltaVelocity;
		playerVelocity.x = playerVelocity.x * drag;

		sf::Vector2f deltaPositionTwo = playerTwoVelocity * deltaTime;
		playerTwoPosition += deltaPositionTwo;
		sf::Vector2f deltaVelocityTwo = playerTwoAcceleration * deltaTime;
		playerTwoVelocity += deltaVelocityTwo;
		playerTwoVelocity.x = playerTwoVelocity.x * drag;

		playerSprite.setPosition(playerPosition);
		playerTwoSprite.setPosition(playerTwoPosition);
		playerCenter = playerPosition + 0.5f * spriteSize;
		playerTwoCenter = playerTwoPosition + 0.5f * spriteSize;

		//Rects
		sf::FloatRect playerRect = playerSprite.getGlobalBounds();
		sf::Vector2f playerRectSize = sf::Vector2f(playerRect.width, playerRect.height);

		sf::FloatRect playerTwoRect = playerTwoSprite.getGlobalBounds();
		sf::Vector2f playerTwoRectSize = sf::Vector2f(playerTwoRect.width, playerTwoRect.height);

		//Geometry

		playerRect.left += playerBoundsOffset.x;
		playerRect.top += playerBoundsOffset.y;
		playerRect.width *= playerBoundsScale.x;
		playerRect.height *= playerBoundsScale.y;

		playerTwoRect.left += playerTwoBoundsOffset.x;
		playerTwoRect.top += playerTwoBoundsOffset.y;
		playerTwoRect.width *= playerTwoBoundsScale.x;
		playerTwoRect.height *= playerTwoBoundsScale.y;

		//Update visuals
		//playerCircle.setPosition(playerPosition);

		playerRectDisplay.setPosition(playerRect.left, playerRect.top);
		playerTwoRectDisplay.setPosition(playerTwoRect.left, playerTwoRect.top);

		playerRectDisplay.setSize(sf::Vector2f(playerRect.width, playerRect.height));
		playerTwoRectDisplay.setSize(sf::Vector2f(playerTwoRect.width, playerTwoRect.height));

		if (playerRect.intersects(playerTwoRect))
		{
			Intersect(playerRect, playerTwoRect, playerCenter, playerTwoCenter, colDepth, playerPosition, playerVelocity, playerRectDisplay, playerTwoRectDisplay);
			playerSprite.setPosition(playerPosition);
			playerTwoSprite.setPosition(playerTwoPosition);
		}
		else
		{
			sf::Color fillcolor = sf::Color::Green;
			fillcolor.a = 125;

			playerRectDisplay.setFillColor(fillcolor);
			playerTwoRectDisplay.setFillColor(fillcolor);
		}

		//UPDATE PIPS
		//Practical Task 3 - Gravity Prediction
		//Update projectile prediction values (MIGHT NEED THIS!)
		initialPosition = playerPosition;

		sf::Vector2f mousePosition = sf::Vector2f(sf::Mouse::getPosition(window));
		firingDirection = mousePosition - initialPosition;

		//Get the size of the vector (magnitude)
		//Normalise firingDirection to size of 1. (unit vector)
		float magnitude = sqrt(firingDirection.x * firingDirection.x + firingDirection.y * firingDirection.y);
		firingDirection /= magnitude;

		//Initial velocity
		initialVelocity = firingDirection * firingSpeed;

		float pipTime = 0;
		for (size_t i = 0; i < pips.size(); ++i)
		{
			pipTime += 0.1f; //Add 1/10th of a second
			sf::Vector2f pipPosition;
			//pipPosition = sf::Vector2f(300, 1000) * pipTime * pipTime + sf::Vector2f(500, -1000) * pipTime + sf::Vector2f(500, 500);
			//Practical Task 3 - Gravity Prediction
			pipPosition = pipTime * initialVelocity + (gravityVector * pipTime * pipTime) / 2.0f + initialPosition;
			pips[i].setPosition(pipPosition);
		}

		grenadeVelocity += gravity * frameTime.asSeconds();
		grenadeSprite.setPosition(grenadeSprite.getPosition() + grenadeVelocity * frameTime.asSeconds());

		for (int i = 0; i < crateSprites.size(); ++i)
		{
			sf::FloatRect crateRect = crateSprites[i].getGlobalBounds();
			sf::Vector2f crateCenter = crateSprites[i].getPosition() + 0.5f * crateSpriteSize;
			sf::FloatRect grenadeRect = grenadeSprite.getGlobalBounds();
			sf::FloatRect playerRect = playerSprite.getGlobalBounds();
			sf::FloatRect playerTwoRect = playerTwoSprite.getGlobalBounds();

			sf::Vector2f normal;

			if (playerRect.intersects(crateRect))
			{
				Intersect(playerRect, crateRect, playerCenter, crateCenter, colDepth, playerPosition, playerVelocity, playerRectDisplay, playerTwoRectDisplay);
			}

			//"Practical Task 5 - Reflection"
			if (crateRect.intersects(grenadeRect))
			{
				//Ricochet
				//Find the side of collision using our collision depth
				sf::Vector2f depth = CollisionDepth(grenadeRect, crateRect);
				sf::Vector2f absDepth = sf::Vector2f(abs(depth.x), abs(depth.y));
				if (absDepth.x < absDepth.y)
				{
					sf::Vector2f grenadePos = grenadeSprite.getPosition();
					grenadePos.x += depth.x;
					grenadeSprite.setPosition(grenadePos);

					//Are we colliding from the left or right
					if (depth.x > 0) //colliding from the left
					{
						//set the normal vector
						normal = sf::Vector2f(-1, 0);
					}
					else //Colliding from the right
					{
						normal = sf::Vector2f(1, 0);
					}
				}
				else //Colliding in the y direction
				{
					//Move out of the collision first of all
					sf::Vector2f grenadePos = grenadeSprite.getPosition();
					grenadePos.y += depth.y;
					grenadeSprite.setPosition(grenadePos);

					if (depth.y > 0)
					{
						normal = sf::Vector2f(0, -1);
					}
					else
					{
						normal = sf::Vector2f(0, 1);
					}
				}
				//"Practical Task 5 - Reflection"
				//Apply the reflection equation
			//R = I - 2N(I * N)
			//R = out going velocity
			//I = incoming velocity
			//Normal vector
				grenadeVelocity = grenadeVelocity - 2.0f * normal * (VectorDot(grenadeVelocity, normal));
			}		
		}

		window.clear();
		window.draw(playerSprite);
		window.draw(playerTwoSprite);
		window.draw(playerRectDisplay);
		window.draw(playerTwoRectDisplay);
		//window.draw(uiSprite);

		for (size_t i = 0; i < pips.size(); ++i)
		{
			window.draw(pips[i]);
		}

		for (size_t i = 0; i < NUM_CRATES; ++i)
		{
			window.draw(crateSprites[i]);
		}

		window.draw(grenadeSprite);

		window.display();
	}
	return 0;
}


void PlayerOneInput(sf::Vector2f & _playerAcceleration, float _ACCEL_RATE, float & _jumpForce)
{
	_playerAcceleration.x = 0;
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
	_playerTwoAcceleration.x = 0;
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

void PlayerMovement(sf::Vector2f & _playerVelocity, sf::Vector2f & _playerAcceleration, float & _deltaTime, sf::Vector2f & _playerPosition, float & _drag)
{
	//std::cout << "After swap, value of b :" << std::endl;
	sf::Vector2f deltaPosition = _playerVelocity * _deltaTime;
	_playerPosition += deltaPosition;
	sf::Vector2f deltaVelocity = _playerAcceleration * _deltaTime;
	_playerVelocity += deltaVelocity;
	_playerVelocity.x = _playerVelocity.x * _drag;

}

bool IntersectCheck(sf::Vector2f _spherePos, sf::Vector2f _otherPos, float _radius)
{
	float distance = sqrt((_spherePos.x - _otherPos.x) * (_spherePos.x - _otherPos.x) +
		(_spherePos.y - _otherPos.y) * (_spherePos.y - _otherPos.y));

	return (distance < (_radius + _radius));
}

void Intersect(sf::FloatRect & _playerRect, sf::FloatRect & _playerTwoRect, sf::Vector2f & _playerCenter, sf::Vector2f & _playerTwoCenter, sf::Vector2f & _colDepth, sf::Vector2f & _playerPos, sf::Vector2f & _playerVelocity, sf::RectangleShape & _playerRectDisplay, sf::RectangleShape & _playerTwoRectDisplay)
{
	sf::Color fillcolor = sf::Color::Red;
	fillcolor.a = 125;

	//playerCircle.setFillColor(fillcolor);
	//enemyCircle.setFillColor(fillcolor);

	_playerRectDisplay.setFillColor(fillcolor);
	_playerTwoRectDisplay.setFillColor(fillcolor);
	
	// Collision Depth
	sf::Vector2f AWidthHeight = sf::Vector2f(_playerRect.width, _playerRect.height);
	sf::Vector2f BWidthHeight = sf::Vector2f(_playerTwoRect.width, _playerTwoRect.height);

	sf::Vector2f ACentre = sf::Vector2f(_playerRect.left, _playerRect.top);
	ACentre += 0.5f * AWidthHeight;
	sf::Vector2f BCentre = sf::Vector2f(_playerTwoRect.left, _playerTwoRect.top);
	BCentre += 0.5f * BWidthHeight;

	sf::Vector2f distance = BCentre - ACentre;
	sf::Vector2f minDistance = 0.5f * AWidthHeight + 0.5f * BWidthHeight;
	if (distance.x < 0)
		minDistance.x = -minDistance.x;
	if (distance.y < 0)
		minDistance.y = -minDistance.y;

	sf::Vector2f depth = minDistance - distance;
	sf::Vector2f absDepth = sf::Vector2f(abs(depth.x), abs(depth.y));

	if (absDepth.x < absDepth.y)
	{
		// Move along x direction
		_playerPos.x -= depth.x;

		// Stop movement in x direction
		_playerVelocity.x = 0;
	}
	else
	{
		// Move along y direction
		_playerPos.y -= depth.y;

		// Stop movement in y direction
		_playerVelocity.y = 0;
	}
}

void QuadEaseOut(sf::Vector2f& _change, float& _time, float& _duration, float& _deltaTime, sf::Vector2f& _begin, sf::Vector2f& _end, sf::Vector2f& _uiPosition)
{
	if (_time < _duration)
	{
		_time += _deltaTime;
		sf::Vector2f k1 = _change / (3 * _duration * _duration);
		sf::Vector2f k2 = (2.0f * _change * _time * _time) / (3 * _duration);
		sf::Vector2f k3 = _begin;
		_uiPosition = k1 * _time * _time + k2 * _time + k3;
	}
	else
	{
		_uiPosition = _end; 
	}
}