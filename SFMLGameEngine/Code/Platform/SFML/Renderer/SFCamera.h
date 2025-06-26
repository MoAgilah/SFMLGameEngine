#pragma once

#include "../../../Engine/Interfaces/ICamera.h"
#include <SFML/Graphics.hpp>

class SFCamera : public ICamera
{
public:
    SFCamera();
    ~SFCamera() override = default;

    void Update() override;
    void Reset(INativeWindow& window) override;
    void RenderDebug(INativeWindow& window) override;

    bool IsInView(BoundingVolume* volume) override;
    bool CheckVerticalBounds(BoundingBox* box) override;

    sf::View GetView() const { return m_camera; }

private:
    sf::View m_camera;
    BoundingBox m_viewBox;
};
