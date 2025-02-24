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
    // Fixed time step (e.g., for 60 updates per second)
    const float dt = 1.f / GameConstants::FPS;
    const int subSteps = 4;  // Number of sub-steps for improved collision accuracy
    const float subStepDt = dt / static_cast<float>(subSteps);

    auto& window = m_gameMgr.GetRenderWindow();
    window.create(sf::VideoMode(static_cast<int>(GameConstants::ScreenDim.x),
        static_cast<int>(GameConstants::ScreenDim.y)),
        GameConstants::WindowTitle);

    window.setFramerateLimit(static_cast<int>(GameConstants::FPS));

    sf::Clock clock;
    float accumulator = 0.0f;

    while (window.isOpen())
    {
        // Process events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            else if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();

                m_gameMgr.GetInputManager().ProcessKeyPressedEvent(event);
            }
            else if (event.type == sf::Event::KeyReleased)
            {
                m_gameMgr.GetInputManager().ProcessKeyReleasedEvent(event);
            }
        }

        // Calculate elapsed time for this frame
        float frameTime = clock.restart().asSeconds();

        // Clamp frameTime to avoid instability during long frames
        if (frameTime > 0.25f)
            frameTime = 0.25f;
        accumulator += frameTime;

        // Sub-stepping update loop for better collision accuracy
        while (accumulator >= dt)
        {
            for (int i = 0; i < subSteps; ++i)
                m_gameMgr.Update(subStepDt);  // Perform sub-step updates

            accumulator -= dt;
        }

        // Render once per frame
        window.clear(GameConstants::WindowColour);
        m_gameMgr.Render();
        window.display();
    }

    return 0;
}