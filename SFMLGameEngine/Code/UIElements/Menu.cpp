#include "Menu.h"
#include "../Game/GameManager.h"

Menu::Menu(std::function<void(int)> func, std::string fontName, TextType textType, const std::string text, unsigned int charSize, unsigned int marginSize, const sf::Vector2f& pos, sf::Color color)
	: m_actionFunc(func), m_charSize(charSize), m_marginSize(m_charSize + marginSize), m_position(pos), m_fontName(fontName), m_textType(textType), m_activeColour(color)
{
	m_menuItems.push_back(CreateMenuItem(text, m_activeColour));
	m_position.y += m_marginSize;
}

void Menu::AddMenuItem(const std::string text)
{
	sf::Color itemColor = m_hasPassiveColor ? m_passiveColour : m_activeColour;
	m_menuItems.push_back(CreateMenuItem(text, itemColor));
	m_position.y += m_marginSize;
}

void Menu::ProcessInput()
{
	HandleNavigation();
}

Text Menu::CreateMenuItem(const std::string& text, sf::Color color)
{
	Text menuItem(m_fontName);
	switch (m_textType)
	{
	case Static:
		menuItem.InitStaticText(text, m_charSize, m_position, color);
		break;
	case Dynamic:
		if (m_initial)
		{
			menuItem.InitFlashingText(text, m_charSize, m_position, color);
			m_initial = false;
		}
		else
		{
			menuItem.InitFlashingText(text, m_charSize, m_position, color, None, true);
		}
		break;
	}

	return menuItem;
}
void Menu::HandleNavigation()
{
	auto& inputManager = GameManager::Get()->GetInputManager();

	HandleDirection(inputManager.GetKeyState(sf::Keyboard::Up), m_canGoUp, -1);
	HandleDirection(inputManager.GetKeyState(sf::Keyboard::Down), m_canGoDown, 1);

	if (inputManager.GetKeyState(sf::Keyboard::Enter))
	{
		m_actionFunc(m_menuPosition);
	}
}

void Menu::HandleDirection(bool isPressed, bool& canMove, int direction)
{
	if (isPressed)
	{
		if (canMove)
		{
			m_menuPosition += direction;
			canMove = false;
			m_menuMoved = true;
		}
	}
	else
	{
		canMove = true;
	}
}

void Menu::ClampMenuPosition()
{
	int maxIndex = static_cast<int>(m_menuItems.size()) - 1;
	m_menuPosition = std::clamp(m_menuPosition, 0, maxIndex);
}

void Menu::Update(float deltaTime)
{
	if (m_menuMoved)
	{
		ClampMenuPosition();

		for (int i = 0; i < m_menuItems.size(); ++i)
		{
			if (i == m_menuPosition)
				m_menuItems[i].Resume();
			else
				m_menuItems[i].Pause();
		}

		m_menuMoved = false;
	}

	for (auto& menuItem : m_menuItems)
	{
		menuItem.Update(deltaTime);
	}
}

void Menu::Render(sf::RenderWindow& window)
{
	for (auto& menuItem : m_menuItems)
	{
		menuItem.Render(window);
	}
}

void Menu::SetPassiveColour(sf::Color color)
{
	m_passiveColour = color;
	m_hasPassiveColor = m_activeColour == m_passiveColour;
}