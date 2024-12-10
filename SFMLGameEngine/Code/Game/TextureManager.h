#pragma once

#include <map>
#include <SFML/Graphics.hpp>

enum class TexID
{
	None = -1,
};

class TextureManager
{
public:
	TextureManager() = default;
	~TextureManager() = default;

	const sf::Texture& GetTexture(TexID id);
	void AddTextures(TexID id);

private:

	std::map<TexID, sf::Texture> m_textureMap;
};