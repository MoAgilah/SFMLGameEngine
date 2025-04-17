#include "MenuItem.h"

#include "../Game/GameManager.h"

MenuItem::MenuItem(const std::string& fontName, const TextConfig& config, bool paused)
	: m_menuItemType(MenuItemType::TextOnly), m_paused(paused)
{
	AssignText(fontName, config);
}

MenuItem::MenuItem(const std::string& texId, const Point& position, bool paused)
	: m_menuItemType(ImageOnly), m_paused(paused)
{
	m_sprite = std::make_unique<Sprite>(texId);
	SetPosition(position);
}

MenuItem::MenuItem(const std::string& fontName, const TextConfig& config, const std::string& texId, const Point& position, bool paused)
	: m_menuItemType(Combination), m_paused(paused)
{
	m_sprite = std::make_unique<Sprite>(texId);
	AssignText(fontName, config);
	SetPosition(position);
}

MenuItem::MenuItem(const std::string& fontName, const TextConfig& config, const std::string& texId, SpriteAnchorPos anchor, bool paused, float gap)
	: m_menuItemType(Combination), m_paused(paused)
{
	m_sprite = std::make_unique<Sprite>(texId);
	AssignText(fontName, config);
	CalculateSpritePosition(anchor, gap);
}

void MenuItem::Update(float deltaTime)
{
	if (m_text)
		m_text->Update(deltaTime);

	if (m_sprite)
		m_sprite->Update(deltaTime);
}

void MenuItem::Render()
{
	sf::RenderWindow& window = GameManager::Get()->GetRenderWindow();

	if (m_text)
		m_text->Render(window);

	if (m_sprite)
		m_sprite->Render(window);
}

void MenuItem::AssignText(const std::string& fontName, const TextConfig& config)
{
	switch (config.m_animType)
	{
	case Static:
		m_text = std::make_unique<Text>(fontName, config);
		break;
	case Flashing:
		m_text = std::make_unique<AnimatedText>(fontName, config);
		break;
	case Custom:
		m_text = std::make_unique<AnimatedText>(fontName, config);
		break;
	}
}

void MenuItem::SetPosition(const Point& position)
{
	Point spriteSize = m_sprite->GetSize();
	m_sprite->SetOrigin(spriteSize / 2.f);
	m_sprite->SetPosition(position);
}

void MenuItem::CalculateSpritePosition(SpriteAnchorPos anchor, float gap)
{
	Point textPos = m_text->GetPosition();
	Point textSize = m_text->GetSize();

	Point spriteSize = m_sprite->GetSize();

	Point pos;
	pos.x = textPos.x + (textSize.x - spriteSize.x) / 2;

	switch (anchor)
	{
	case Centered:
		pos.y = textPos.y + (textSize.y - spriteSize.y) / 2;
		break;
	case Above:
		pos.y = textPos.y - spriteSize.y - gap;
		break;
	case Below:
		pos.y = textPos.y + textSize.y + gap;
		break;
	}

	SetPosition(pos);
}