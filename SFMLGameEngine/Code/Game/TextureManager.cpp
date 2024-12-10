#include "TextureManager.h"
#include <iostream>
#include <format>
#include <stdexcept>

std::string TexIDToString(TexID id)
{
	switch (id)
	{
	default: return "";
	}
}

const sf::Texture& TextureManager::GetTexture(TexID id)
{
	auto it = m_textureMap.find(id);
	if (it == m_textureMap.end())
	{
		throw std::invalid_argument(std::format("Texture {} not recognised", (int)id));
		return sf::Texture();
	}

	return m_textureMap[id];
}

void TextureManager::AddTextures(TexID id)
{
	sf::Texture texture;
	if (!texture.loadFromFile(std::format("../Resources/Sprites/{}.png", TexIDToString(id))))
	{
		std::cout << std::format("failed to load ../Resources/Sprites/{}.png", TexIDToString(id)) << std::endl;
	}

	texture.setSmooth(true);
	m_textureMap.try_emplace(id, texture);
}