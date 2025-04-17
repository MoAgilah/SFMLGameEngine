#include "Menu.h"
#include "../Game/GameManager.h"

Menu::Menu(std::function<void(int)> func, std::string fontName, TextAnimType textType, const std::string text, unsigned int charSize, unsigned int marginSize, const Point& pos, sf::Color color)
	: m_actionFunc(func), m_charSize(charSize), m_marginSize(m_charSize + marginSize), m_position(pos), m_fontName(fontName), m_textType(textType), m_activeColour(color)
{
	Text* item = CreateMenuItem(text, m_activeColour);
	m_menuItems.emplace_back(std::move(item));
	m_position.y += m_marginSize;
}

void Menu::AddMenuItem(const std::string text)
{
	sf::Color itemColor = m_hasPassiveColor ? m_passiveColour : m_activeColour;
	Text* item = CreateMenuItem(text, itemColor);
	m_menuItems.emplace_back(std::move(item));
	m_position.y += m_marginSize;
}

void Menu::ProcessInput()
{
	HandleNavigation();
}

Text* Menu::CreateMenuItem(const std::string& text, sf::Color color)
{
	Text* menuItem = nullptr;
	switch (m_textType)
	{
	case Static:
		menuItem = new Text(m_fontName, TextConfig(m_charSize, m_position, m_textType, color));
		menuItem->SetText(text);
		break;
	case Flashing:
		menuItem = new AnimatedText(m_fontName, TextConfig(m_charSize, m_position, m_textType, color));
		if (m_initial)
		{
			m_initial = false;
			dynamic_cast<AnimatedText*>(menuItem)->InitFlashingText(text, true);

		}
		else
		{
			auto aniText = dynamic_cast<AnimatedText*>(menuItem);
			aniText->InitFlashingText(text, true);
			aniText->Pause();
		}
		break;
	}

	return std::move(menuItem);
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

		if (m_textType > Static)
		{
			for (int i = 0; i < m_menuItems.size(); ++i)
			{
				auto aniText = dynamic_cast<AnimatedText*>(m_menuItems[i].get());
				if (i == m_menuPosition)
					aniText->Resume();
				else
					aniText->Pause();
			}
		}

		m_menuMoved = false;
	}

	for (auto& menuItem : m_menuItems)
		menuItem->Update(deltaTime);
}

void Menu::Render(sf::RenderWindow& window)
{
	for (auto& menuItem : m_menuItems)
		menuItem->Render(window);
}

void Menu::SetPassiveColour(sf::Color color)
{
	m_passiveColour = color;
	m_hasPassiveColor = m_activeColour == m_passiveColour;
}