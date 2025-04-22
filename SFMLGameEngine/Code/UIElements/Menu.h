#pragma once

#include "MenuItem.h"
#include <optional>
#include <functional>
#include <map>

class Menu
{
public:
	virtual ~Menu() = default;

	void Start();
	void Pause();
	void Resume();

	void ProcessInput();
	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	void SetHorizontalScrolling() { m_verticalScroll = false; }

	void AddCursor(Sprite* cursor, SpriteAnchorData cursorAnchorData, bool toText = true);

private:

	void HandleNavigation();

	void HandleDirection(bool isPressed, bool& canMove, int direction);

	void ClampMenuPosition();

	bool m_canIncMenu = true;
	bool m_canDecMenu = false;

	int m_prevMenuPosition = 0;

protected:

	Point GetNextDrawablePosition(const Point& size, const Point& pos);

	bool m_verticalScroll = true;
	MenuItemType m_menuType;
	int m_menuPosition = 0;
	std::function<void(int)> m_actionFunc;
	unsigned int m_marginSize = 0;
	bool m_anchorToText = true;
	std::shared_ptr<Sprite> m_cursor;
	std::optional<SpriteAnchorData> m_cursorAnchorData = std::nullopt;
	std::vector<std::unique_ptr<MenuItem>> m_menuItems;
};

class TextBasedMenu : public Menu
{
public:
	TextBasedMenu(std::function<void(int)> func, const std::string& text, const TextConfig& config, unsigned int marginSize, std::optional<sf::Color> passiveColour = std::nullopt);
	~TextBasedMenu() = default;

	void AddMenuItem(const std::string& text);

private:

	TextConfig m_textConfig;
	std::optional<sf::Color> m_passiveColour;
};

class ImageBasedMenu : public Menu

{
public:
	ImageBasedMenu(std::function<void(int)> func, const std::string& texID, const Point& imgPos, unsigned int marginSize);
	~ImageBasedMenu() = default;

	void AddMenuItem(const std::string& texID);
};

class TextImageBasedMenu : public Menu
{
public:
	TextImageBasedMenu(std::function<void(int)> func, const std::string& text, const TextConfig& config, std::optional<sf::Color> passiveColour,
		const std::string& texID, const Point& imgPos, unsigned int marginSize);

	TextImageBasedMenu(std::function<void(int)> func, const std::string& text, const TextConfig& config, std::optional<sf::Color> passiveColour,
		const std::string& texID, const SpriteAnchorData& AnchorData, unsigned int marginSize);

	~TextImageBasedMenu() = default;

	void AddMenuItem(const std::string& text, const std::string& filename);

private:

	TextConfig m_textConfig;
	std::optional<sf::Color> m_passiveColour;
	std::optional<SpriteAnchorData> m_anchorData;
};