#include "MenuItem.h"

#include "../Game/GameManager.h"

MenuItem::MenuItem(const std::string& text, const TextConfig& config, bool paused)
	: m_menuItemType(MenuItemType::TextOnly), m_textConfig(config)
{
	AssignText(text, m_textConfig.value(), paused);
}

MenuItem::MenuItem(const std::string& texId, const Point& position, bool paused)
	: m_menuItemType(ImageOnly), m_textConfig(std::nullopt)
{
	m_sprite = std::make_unique<Sprite>(texId);
	SetPosition(position);
}

MenuItem::MenuItem(const std::string& text, const TextConfig& config, const std::string& texId, const Point& position, bool paused)
	: m_menuItemType(ImagePositioned), m_textConfig(config)
{
	m_sprite = std::make_unique<Sprite>(texId);
	AssignText(text, m_textConfig.value(), paused);
	SetPosition(position);
}

MenuItem::MenuItem(const std::string& text, const TextConfig& config, const std::string& texId, const SpriteAnchorData& anchorData, bool paused)
	: m_menuItemType(ImageAnchored)
{
	m_sprite = std::make_unique<Sprite>(texId);
	AssignText(text, m_textConfig.value(), paused);
	CalculateSpritePosition(anchorData);
}

void MenuItem::Update(float deltaTime)
{
	if (m_text)
		m_text->Update(deltaTime);

	if (m_sprite)
		m_sprite->Update(deltaTime);
}

void MenuItem::Render(sf::RenderWindow& window)
{
	if (m_text)
		m_text->Render(window);

	if (m_sprite)
		m_sprite->Render(window);
}

void MenuItem::Pause()
{
	if (m_text)
	{
		auto aniText = dynamic_cast<AnimatedText*>(m_text.get());
		if (aniText)
		{
			aniText->Pause();
		}
	}
}

void MenuItem::Resume()
{
	if (m_text)
	{
		auto aniText = dynamic_cast<AnimatedText*>(m_text.get());
		if (aniText)
		{
			aniText->Resume();
		}
	}
}

void MenuItem::AssignText(const std::string& text, const TextConfig& config, bool paused)
{
	switch (config.m_animType)
	{
	case Static:
		m_text = std::make_unique<Text>(config);
		m_text->SetText(text);
		break;
	case Flashing:
		m_text = std::make_unique<AnimatedText>(config);
		m_text->SetText(text);
		if (paused)
			dynamic_cast<AnimatedText*>(m_text.get())->Pause();
		break;
	case Custom:
		m_text = std::make_unique<AnimatedText>(config);
		m_text->SetText(text);
		if (paused)
			dynamic_cast<AnimatedText*>(m_text.get())->Pause();
		break;
	}
}

void MenuItem::SetPosition(const Point& position)
{
	Point spriteSize = m_sprite->GetSize();
	m_sprite->SetOrigin(spriteSize / 2.f);
	m_sprite->SetPosition(position);
}

void MenuItem::CalculateSpritePosition(const SpriteAnchorData& anchorData)
{
	Point textPos = m_text->GetPosition();
	Point textSize = m_text->GetSize();

	Point spriteSize = m_sprite->GetSize();

	Point pos;
	pos.x = textPos.x + (textSize.x - spriteSize.x) / 2;

	switch (anchorData.m_imgAnchor)
	{
	case Centered:
		pos.y = textPos.y + (textSize.y - spriteSize.y) / 2;
		break;
	case Above:
		pos.y = textPos.y - spriteSize.y - anchorData.m_margin;
		break;
	case Below:
		pos.y = textPos.y + textSize.y + anchorData.m_margin;
		break;
	}

	SetPosition(pos);
}