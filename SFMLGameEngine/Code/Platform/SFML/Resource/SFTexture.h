#pragma once

#include "../../../Engine/Interfaces/ITexture.h"
#include "../../../Utilities/Traits.h"
#include <SFML/Graphics.hpp>

class SFTexture : public ITexture
{
public:
	bool LoadFromFile(const std::string& filepath) override
	{
        return m_texture.loadFromFile(filepath);
	}

    sf::Texture& GetNativeTexture() { return m_texture; }

private:
    sf::Texture m_texture;
};

template<>
struct TextureTrait<SFTexture>
{
    using TextureType = SFTexture;

    static std::unique_ptr<ITexture> Create()
    {
        return std::make_unique<SFTexture>();
    }
};

