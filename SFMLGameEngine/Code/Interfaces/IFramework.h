#pragma once

#include "Game/GameManager.h"

class IFrameWork
{
public:
    virtual ~IFrameWork() = default;

    // Entry point
    int Run();

    // Life cycle hooks
    virtual void Initialise() = 0;
    virtual void LoadContent() = 0;
    virtual void PollEvents() = 0;
    virtual void Update(float dt) = 0;
    virtual void Render() = 0;
    virtual void Shutdown() = 0;

protected:

    bool m_isRunning = true;
    GameManager m_gameMgr;
};