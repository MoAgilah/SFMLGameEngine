#include "SFMLWindow.h"

#include "../Game/Constants.h"
#include "../Game/GameManager.h"


bool SFMLWindow::Create(const Point& screemDims, const std::string& title)
{
	GetWindow()->create(sf::VideoMode(screemDims), title);
	return m_window->isOpen();
}

void SFMLWindow::PollEvents()
{
	while (auto event = m_window->pollEvent())
	{
		if (event.has_value())
		{
			if (event->is<sf::Event::Closed>())
			{
				Close();
			}
			else if (auto keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
					Close();

				GameManager::Get()->GetInputManager().ProcessKeyPressedEvent(keyPressed);
			}
			else if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>())
			{
				GameManager::Get()->GetInputManager().ProcessKeyReleasedEvent(keyReleased);
			}
		}
	}
}

bool SFMLWindow::ShouldClose() const
{
	return m_shouldClose || !m_window->isOpen();
}

void SFMLWindow::Close()
{
	if (m_window)
		m_window->close();

	m_shouldClose = true;
}

void* SFMLWindow::GetNativeHandle()
{
   return reinterpret_cast<void*>(m_window->getNativeHandle());
}

sf::RenderWindow* SFMLWindow::GetWindow()
{
	return m_window.get();
}
