#pragma once

#include <cstdint>

#define USE_SFML 1

struct Colour
{
    uint8_t r{}, g{}, b{}, a{ 255 };

    Colour() = default;
    Colour(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255)
        : r(red), g(green), b(blue), a(alpha)
    {
    }

#if USE_SFML
#include <SFML/Graphics/Color.hpp>
    operator sf::Color() const { return sf::Color(r, g, b, a); }

    Colour(const sf::Color& c) : r(c.r), g(c.g), b(c.b), a(c.a) {}

    sf::Color ToSFML() const { return sf::Color(r, g, b, a); }
#endif
};
