#include "Framework.h"
#include <SFML/Graphics.hpp>

FrameWork::FrameWork(const std::string& windowTitle)
{
	GameConstants::WindowTitle = windowTitle;
}

void FrameWork::SetInitialGameState(GameState* state)
{
	m_gameMgr.GetGameStateMgr()->ChangeState(state);
}

int FrameWork::Run()
{
	float t = 0.0f;
	float dt = 1.f / GameConstants::FPS;

	auto& window = m_gameMgr.GetRenderWindow();

	window.create(sf::VideoMode((int)GameConstants::ScreenDim.x, (int)GameConstants::ScreenDim.y), GameConstants::WindowTitle);
	window.setFramerateLimit(GameConstants::FPS);

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
					window.close();

				m_gameMgr.GetInputManager().ProcessKeyPressedEvent(event);
			}

			if (event.type == sf::Event::KeyReleased)
			{
				m_gameMgr.GetInputManager().ProcessKeyReleasedEvent(event);
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
			m_gameMgr.Update(deltaTime);
			//end update

			frameTime -= deltaTime;
			t += deltaTime;
		}

		//do render
		m_gameMgr.Render();
		//end render

		window.display();
	}

	return 0;
}
