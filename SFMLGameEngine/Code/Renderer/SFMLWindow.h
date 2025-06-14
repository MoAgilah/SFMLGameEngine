#pragma once

#include "../Interfaces/IWindow.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class SFMLWindow : public IWindow<sf::RenderWindow>
{
public:
    bool Create(const Point& screemDims, const std::string& title) override;
    void PollEvents() override;
    bool ShouldClose() const override;
    void Close() override;
    void* GetNativeHandle() override;

    sf::RenderWindow* GetWindow();

private:
    bool m_shouldClose = false;
};
