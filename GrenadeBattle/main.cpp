#include <SFML/Graphics.hpp>
#include <Math.h>
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>

void PlayerOneInput(sf::Vector2f & _playerAcceleration, float _ACCEL_RATE, float & _jumpForce);
void PlayerTwoInput(sf::Vector2f & _playerTwoAcceleration, float _ACCEL_RATE, float & _jumpForce);
void PlayerMovement(sf::Vector2f & _playerVelocity, sf::Vector2f & _playerAcceleration, float _deltaTime, sf::Vector2f & _playerPosition, float _drag);
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

	//Practical Task 3 - Gravity Prediction
	sf::Vector2f initialVelocity(500, -1000);
	sf::Vector2f initialPosition;
	sf::Vector2f gravityVector(300, 1000);

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
	sf::Vector2f playerVelocity = sf::Vector2f(200.0f, 0.0f);
	sf::Vector2f terminalVelocity = sf::Vector2f(0.0f, 70.0f);
	
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
		//Game Clock
		sf::Time frameTime = gameClock.restart();
		float deltaTime = frameTime.asSeconds();
		QuadEaseOut(change, time, duration, deltaTime, begin, end, uiPosition);
		uiSprite.setPosition(uiPosition);
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
		initialPosition = playerPosition;
		float pipTime = 0;
		for (size_t i = 0; i < pips.size(); ++i)
		{
			pipTime += 0.1f; //Add 1/10th of a second
			sf::Vector2f pipPosition;
			//pipPosition = sf::Vector2f(300, 1000) * pipTime * pipTime + sf::Vector2f(500, -1000) * pipTime + sf::Vector2f(500, 500);
			//Practical Task 3 - Gravity Prediction
			pipPosition = pipTime * initialVelocity + gravityVector * pipTime * pipTime + initialPosition;
			pips[i].setPosition(pipPosition);
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