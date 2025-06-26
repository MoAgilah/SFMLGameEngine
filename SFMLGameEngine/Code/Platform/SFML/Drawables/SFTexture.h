#pragma once

#include "../../../Engine/Interfaces/ITexture.h"
#include <SFML/Graphics.hpp>
#include <memory>

class SFTexture : public ITexture
{
public:
	SFTexture();

	bool LoadFromFile(const std::string& path) override;

	Point GetTextureSize() const override;

    sf::Texture* GetTexture()
    {
        return m_texture.get();
    }

protected:
    void* GetNativeHandle() override {
        return reinterpret_cast<void*>(m_texture.get());
    }

private:

    std::shared_ptr<sf::Texture> m_texture;
};