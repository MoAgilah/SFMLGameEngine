#include "Framework.h"
#include <SFML/Graphics.hpp>

FrameWork::FrameWork(const std::string& windowTitle)
{
	GameConstants::WindowTitle = windowTitle;
}

void FrameWork::Initialise()
{
}

int FrameWork::Run()
{
    float t = 0.0f;
    float dt = 1.f / GameConstants::FPS;  // Fixed time step

    auto& window = m_gameMgr.GetRenderWindow();

    window.create(sf::VideoMode((int)GameConstants::ScreenDim.x, (int)GameConstants::ScreenDim.y), GameConstants::WindowTitle);
    window.setFramerateLimit((int)GameConstants::FPS);

    sf::Clock clock;
    sf::Event event;
    float currentTime = clock.getElapsedTime().asSeconds();
    float accumulator = 0.0f;  // Stores leftover time from frames

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

        // Prevent large time steps causing instability
        if (frameTime > 0.25f)
            frameTime = 0.25f;

        accumulator += frameTime;

        // Fixed update loop
        while (accumulator >= dt)
        {
            m_gameMgr.Update(dt);
            accumulator -= dt;
            t += dt;
        }

        // Render once per frame
        window.clear(GameConstants::WindowColour);
        m_gameMgr.Render();
        window.display();
    }

    return 0;
}