#include <SFML/Graphics.hpp>
#include "Game/Constants.h"
#include "Game/GameManager.h"

int main()
{
	float t = 0.0f;
	float dt = 1.f / GameConstants::FPS;
	sf::RenderWindow window(sf::VideoMode((int)GameConstants::ScreenDim.x, (int)GameConstants::ScreenDim.y), "SFML Game Engine");
	window.setFramerateLimit((unsigned int)GameConstants::FPS);

	GameManager gameMgr;

	sf::Clock clock;
	sf::Event event;
	float currentTime = clock.getElapsedTime().asSeconds();

	while (window.isOpen())
	{

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();

			if (event.type == sf::Event::KeyPressed)
			{
				if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
				{
					window.close();
				}
				else
				{
					gameMgr.GetInputManager().ProcessKeyPressedEvent(event);
				}
			}

			if (event.type == sf::Event::KeyReleased)
			{
				gameMgr.GetInputManager().ProcessKeyReleasedEvent(event);
			}
		}

		float newTime = clock.getElapsedTime().asSeconds();
		float frameTime = newTime - currentTime;
		currentTime = newTime;

		window.clear(sf::Color::White);

		while (frameTime > 0.0)
		{
			float deltaTime = std::min(frameTime, dt);

			//do update
			gameMgr.Update(deltaTime);
			//end update

			frameTime -= deltaTime;
			t += deltaTime;
		}

		//do render
		gameMgr.Render(window);
		//end render

		window.display();
	}

	return 0;
}