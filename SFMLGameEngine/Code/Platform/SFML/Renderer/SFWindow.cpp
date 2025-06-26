#include "SFWindow.h"

#include "../../../Engine/Core/Constants.h"
#include "../../../Engine/Core/GameManager.h"


bool SFWindow::Create(const Point& screemDims, const std::string& title)
{
	GetWindow()->create(sf::VideoMode(screemDims), title);
	return m_window->isOpen();
}

void SFWindow::PollEvents()
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

bool SFWindow::ShouldClose() const
{
	return m_shouldClose || !m_window->isOpen();
}

void SFWindow::Close()
{
	if (m_window)
		m_window->close();

	m_shouldClose = true;
}

void* SFWindow::GetNativeHandle()
{
   return reinterpret_cast<void*>(m_window->getNativeHandle());
}

sf::RenderWindow* SFWindow::GetWindow()
{
	return m_window.get();
}
