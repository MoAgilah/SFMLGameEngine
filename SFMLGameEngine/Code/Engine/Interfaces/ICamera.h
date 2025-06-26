#pragma once

#include "IWindow.h"
#include "../Collisions/BoundingVolume.h"

class ICamera
{
public:
    virtual ~ICamera() = default;

    virtual void Update() = 0;
    virtual void Reset(INativeWindow& window) = 0;
    virtual void RenderDebug(INativeWindow& window) = 0;

    virtual bool IsInView(BoundingVolume* volume) = 0;
    virtual bool CheckVerticalBounds(BoundingBox* box) = 0;
};
