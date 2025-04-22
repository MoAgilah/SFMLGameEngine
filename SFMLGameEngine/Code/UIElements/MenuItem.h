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
	ImageAnchored,
	ImagePositioned
};

enum SpriteAnchorPos
{
	Centered,
	Above,
	Below,
};

struct SpriteAnchorData
{
	SpriteAnchorPos m_imgAnchor;
	float m_margin;
};

struct SizePosExtractor
{
	Point m_size;
	Point m_position;

	SizePosExtractor(Text* text)
		: m_size(text->GetSize()), m_position(text->GetPosition())
	{}

	SizePosExtractor(Sprite* spr)
		: m_size(spr->GetSize()), m_position(spr->GetPosition())
	{}
};

void CalculateSpritePosition(SizePosExtractor extracts, Sprite* sprite, const SpriteAnchorData& anchorData);

class MenuItem
{
public:
	MenuItem(const std::string& text, const TextConfig& config, bool paused);
	MenuItem(const std::string& texId, const Point& position, bool paused);
	MenuItem(const std::string& text, const TextConfig& config, const std::string& texId, const Point& position, bool paused);
	MenuItem(const std::string& text, const TextConfig& config, const std::string& texId, const SpriteAnchorData& anchorData, bool paused);
	~MenuItem() = default;

	void Update(float deltaTime);
	void Render(sf::RenderWindow& window);

	Text* GetTextElement() { return m_text.get(); }
	Sprite* GetSpriteElement() { return m_sprite.get(); }

	void Pause();
	void Resume();

private:

	void AssignText(const std::string& text, const TextConfig& config, bool paused);
	void SetImgPosition(const Point& position);

	MenuItemType m_menuItemType;
	std::optional<TextConfig> m_textConfig;
	std::unique_ptr<Text> m_text;
	std::unique_ptr<Sprite> m_sprite;
};