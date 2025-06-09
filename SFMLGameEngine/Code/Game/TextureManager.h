#pragma once

#include "../Code/Utilities/ResourceLoader.h"

class TextureManager
{
public:
	TextureManager();
	~TextureManager() = default;

	sf::Texture* GetTexture(const std::string& name);

	void AddTextures(const fs::path& path);

private:

	ResourceLoader<sf::Texture> m_loader;
};