#pragma once

#include "../Code/Utilities/ResourceLoader.h"

class TextureManager
{
public:
	TextureManager();
	~TextureManager() = default;

	sf::Texture* GetTexture(const std::string& name);

private:

	ResourceLoader<sf::Texture> m_loader;
};