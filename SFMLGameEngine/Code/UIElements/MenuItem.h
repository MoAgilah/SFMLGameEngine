#pragma once

#include "Text.h"
#include "../Drawables/Sprite.h"
#include <string>
#include <variant>
#include <memory>

enum MenuItemType
{
	TextOnly,
	ImageOnly,
	Combination
};

enum SpriteAnchorPos
{
	Centered,
	Above,
	Below,
};

class MenuItem
{
public:
	MenuItem(const std::string& fontName, const TextConfig& config, bool paused);
	MenuItem(const std::string& texId, const Point& position, bool paused);
	MenuItem(const std::string& fontName, const TextConfig& config, const std::string& texId, const Point& position, bool paused);
	MenuItem(const std::string& fontName, const TextConfig& config, const std::string& texId, SpriteAnchorPos anchor, bool paused, float gap = 5.0f);
	~MenuItem() = default;

	void Update(float deltaTime);
	void Render();

	Text* GetTextElement() { return m_text.get(); }
	Sprite* GetSpriteElement() { return m_sprite.get(); }

private:

	void AssignText(const std::string& fontName, const TextConfig& config);
	void SetPosition(const Point& position);
	void CalculateSpritePosition(SpriteAnchorPos anchor, float gap);

	bool m_paused = true;
	MenuItemType m_menuItemType;
	std::unique_ptr<Text> m_text;
	std::unique_ptr<Sprite> m_sprite;
};