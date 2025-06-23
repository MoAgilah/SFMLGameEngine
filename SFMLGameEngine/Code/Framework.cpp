#include "Framework.h"

#include <SFML/Graphics.hpp>
#include <chrono>

FrameWork::FrameWork(const std::string& windowTitle)
{
	GameConstants::WindowTitle = windowTitle;
}

void FrameWork::Initialise()
{
}

int FrameWork::Run()
{
    using clock = std::chrono::steady_clock;
    using duration = std::chrono::duration<float>;

    const float dt = 1.f / GameConstants::FPS;
    const int subSteps = 4;
    const float subStepDt = dt / static_cast<float>(subSteps);

    auto& window = m_gameMgr.GetRenderWindow();
    window.create(sf::VideoMode(sf::Vector2u{
        static_cast<unsigned>(GameConstants::ScreenDim.x),
        static_cast<unsigned>(GameConstants::ScreenDim.y)
        }),
        GameConstants::WindowTitle);

    window.setFramerateLimit(static_cast<int>(GameConstants::FPS));

    float accumulator = 0.0f;
    auto previousTime = clock::now();

    while (window.isOpen())
    {
        // Process events
        while (auto event = window.pollEvent())
        {
            if (event.has_value())
            {
                if (event->is<sf::Event::Closed>())
                {
                    window.close();
                }
                else if (auto keyPressed = event->getIf<sf::Event::KeyPressed>())
                {
                    if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                        window.close();

                    m_gameMgr.GetInputManager().ProcessKeyPressedEvent(keyPressed);
                }
                else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
                {
                    m_gameMgr.GetInputManager().ProcessKeyReleasedEvent(keyReleased);
                }
            }
        }

        // Time step calculation using generic clock
        auto currentTime = clock::now();
        float frameTime = std::chrono::duration_cast<duration>(currentTime - previousTime).count();
        previousTime = currentTime;

        // Clamp frame time
        if (frameTime > 0.25f)
            frameTime = 0.25f;

        accumulator += frameTime;

        // Sub-stepped updates
        while (accumulator >= dt)
        {
            for (int i = 0; i < subSteps; ++i)
                m_gameMgr.Update(subStepDt);

            accumulator -= dt;
        }

        // Render
        window.clear(GameConstants::WindowColour);
        m_gameMgr.Render();
        window.display();
    }

    return 0;
}