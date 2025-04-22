#include "Menu.h"
#include "../Game/GameManager.h"

void Menu::Start()
{
	m_menuPosition = 0;
}

void Menu::Pause()
{
	for (auto& menuItem : m_menuItems)
		menuItem->Pause();
}

void Menu::Resume()
{
	auto& currentMenuItem = m_menuItems[m_menuPosition];
	currentMenuItem->Resume();

	if (m_cursor)
	{
		if (m_anchorToText)
			CalculateSpritePosition(currentMenuItem->GetTextElement(), m_cursor.get(), *m_cursorAnchorData);
		else
			CalculateSpritePosition(currentMenuItem->GetSpriteElement(), m_cursor.get(), *m_cursorAnchorData);
	}

	for (size_t i = 0; i < m_menuPosition; i++)
		m_menuItems[i]->Pause();

	for (size_t i = m_menuPosition + 1; i < m_menuItems.size(); i++)
		m_menuItems[i]->Pause();
}

void Menu::ProcessInput()
{
	HandleNavigation();
}

void Menu::Update(float deltaTime)
{
	if (m_prevMenuPosition != m_menuPosition)
	{
		Resume();



		m_prevMenuPosition = m_menuPosition;
	}

	for (auto& menuItem : m_menuItems)
		menuItem->Update(deltaTime);
}

void Menu::Render(sf::RenderWindow& window)
{
	for (auto& menuItem : m_menuItems)
		menuItem->Render(window);
}

void Menu::AddCursor(Sprite* cursor, SpriteAnchorData cursorAnchorData, bool toText)
{
	m_anchorToText = toText;
	m_cursor = std::shared_ptr<Sprite>(cursor);
	m_cursorAnchorData = cursorAnchorData;
}

void Menu::HandleNavigation()
{
	auto& inputManager = GameManager::Get()->GetInputManager();

	if (m_verticalScroll)
	{
		HandleDirection(inputManager.GetKeyState(sf::Keyboard::Up), m_canIncMenu, -1);
		HandleDirection(inputManager.GetKeyState(sf::Keyboard::Down), m_canDecMenu, 1);
	}
	else
	{
		HandleDirection(inputManager.GetKeyState(sf::Keyboard::Left), m_canIncMenu, -1);
		HandleDirection(inputManager.GetKeyState(sf::Keyboard::Right), m_canDecMenu, 1);
	}

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
			ClampMenuPosition();
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

Point Menu::GetNextDrawablePosition(const Point& size, const Point& pos)
{
	if (m_verticalScroll)
		return Point(pos.x, pos.y + size.y + m_marginSize);
	else
		return Point(pos.x + size.x + m_marginSize, pos.y);
}

TextBasedMenu::TextBasedMenu(std::function<void(int)> func, const std::string& text, const TextConfig& config, unsigned int marginSize, std::optional<sf::Color> passiveColour)
	: m_textConfig(config), m_passiveColour(passiveColour)
{
	m_menuType = TextOnly;
	m_actionFunc = func;
	m_marginSize = marginSize;
	m_cursor = nullptr;

	m_menuItems.push_back(std::make_unique<MenuItem>(text, m_textConfig, false));
}

void TextBasedMenu::AddMenuItem(const std::string& text)
{
	TextConfig config = TextConfig(m_textConfig);

	if (m_passiveColour)
		config.m_colour = *m_passiveColour;

	auto currTextElement = m_menuItems[m_menuPosition]->GetTextElement();
	config.m_position = GetNextDrawablePosition(currTextElement->GetSize(), currTextElement->GetPosition());

	m_menuItems.push_back(std::make_unique<MenuItem>(text, config, true));
	m_menuPosition++;
}

ImageBasedMenu::ImageBasedMenu(std::function<void(int)> func, const std::string& texID, const Point& imgPos, unsigned int marginSize)
{
	m_menuType = ImageOnly;
	m_actionFunc = func;
	m_marginSize = marginSize;
	m_cursor = nullptr;

	m_menuItems.push_back(std::make_unique<MenuItem>(texID, imgPos, false));
}

void ImageBasedMenu::AddMenuItem(const std::string& texID)
{
	auto currSprElement = m_menuItems[m_menuPosition]->GetSpriteElement();
	auto newPos = GetNextDrawablePosition(currSprElement->GetSize(), currSprElement->GetPosition());

	m_menuItems.push_back(std::make_unique<MenuItem>(texID, newPos, true));
	m_menuPosition++;
}

TextImageBasedMenu::TextImageBasedMenu(std::function<void(int)> func, const std::string& text, const TextConfig& config, std::optional<sf::Color> passiveColour,
	const std::string& texId, const Point& imgPos, unsigned int marginSize)
	: m_textConfig(config), m_passiveColour(passiveColour)
{
	m_menuType = ImagePositioned;
	m_actionFunc = func;
	m_marginSize = marginSize;
	m_cursor = nullptr;

	m_menuItems.push_back(std::make_unique<MenuItem>(text, m_textConfig, texId, imgPos, false));
}

TextImageBasedMenu::TextImageBasedMenu(std::function<void(int)> func, const std::string& text, const TextConfig& config,
	std::optional<sf::Color> passiveColour, const std::string& texID, const SpriteAnchorData& anchorData, unsigned int marginSize)
	: m_textConfig(config), m_passiveColour(passiveColour), m_anchorData(anchorData)
{
	m_menuType = ImagePositioned;
	m_actionFunc = func;
	m_marginSize = marginSize;

	m_menuItems.push_back(std::make_unique<MenuItem>(text, m_textConfig, texID, *m_anchorData, false));
}

void TextImageBasedMenu::AddMenuItem(const std::string& text, const std::string& texID)
{
	TextConfig config = TextConfig(m_textConfig);

	if (m_passiveColour)
		config.m_colour = *m_passiveColour;

	auto currTextElement = m_menuItems[m_menuPosition]->GetTextElement();
	config.m_position = GetNextDrawablePosition(currTextElement->GetSize(), currTextElement->GetPosition());

	switch (m_menuType)
	{
	case ImageAnchored:
		m_menuItems.push_back(std::make_unique<MenuItem>(text, config, texID, *m_anchorData, true));
		break;
	case ImagePositioned:
		auto currSprElement = m_menuItems[m_menuPosition]->GetSpriteElement();
		auto imgPos = GetNextDrawablePosition(currSprElement->GetSize(), currSprElement->GetPosition());

		m_menuItems.push_back(std::make_unique<MenuItem>(text, config, texID, imgPos, true));
		break;
	}

	m_menuPosition++;
}