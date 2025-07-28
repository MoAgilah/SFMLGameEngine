#pragma once

#include "../../../Engine/Interfaces/IFont.h"
#include "../../../Utilities/Traits.h"
#include <SFML/Graphics/Font.hpp>

class SFFont : public IFont
{
public:
    bool LoadFromFile(const std::string& filepath) override
    {
        return font.openFromFile(filepath);
    }

    sf::Font& GetNativeFont() { return font; }

private:
    sf::Font font;
};

template<>
struct FontTrait<SFFont>
{
    using FontType = SFFont;

    static std::unique_ptr<IFont> Create()
    {
        return std::make_unique<SFFont>();
    }
};