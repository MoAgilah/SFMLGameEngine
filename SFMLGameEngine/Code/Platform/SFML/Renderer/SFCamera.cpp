#include "SFCamera.h"

#include "../../../Engine/Core/Constants.h"

SFCamera::SFCamera()
{
    const auto& screenDim = GameConstants::ScreenDim;
    const sf::Vector2f center = screenDim * 0.5f;

    m_camera.setSize(screenDim);
    m_camera.setCenter(center);
    m_camera.setViewport({ {0.f, 0.f}, {1.f, 1.f} });

    m_viewBox.Reset(screenDim);
    m_viewBox.Update(center);
    m_viewBox.SetFillColour(sf::Color(255, 0, 0, 128));
}

void SFCamera::Update()
{
    const float posX = 0.f;
    m_camera.setCenter({ posX + (GameConstants::ScreenDim.x * 0.5f), m_camera.getCenter().y });
    m_viewBox.Update(m_camera.getCenter());
}

void SFCamera::Reset(INativeWindow& window)
{
    Update();

    // Downcast to SFML window implementation (safe only if this camera is used with SFML)
    auto* sfmlWindow = static_cast<sf::RenderWindow*>(window.GetNativeHandle());
    if (sfmlWindow)
        sfmlWindow->setView(m_camera);
}

void SFCamera::RenderDebug(INativeWindow& window)
{
    auto* sfmlWindow = static_cast<sf::RenderWindow*>(window.GetNativeHandle());
    if (sfmlWindow)
        m_viewBox.Render(*sfmlWindow);
}

bool SFCamera::IsInView(BoundingVolume* volume)
{
    return m_viewBox.Intersects(volume);
}

bool SFCamera::CheckVerticalBounds(BoundingBox* box)
{
    const float cameraBottom = m_camera.getCenter().y + (GameConstants::ScreenDim.y * 0.5f);
    return box->GetPosition().y > (cameraBottom - (box->GetExtents().y * 2.f));
}
