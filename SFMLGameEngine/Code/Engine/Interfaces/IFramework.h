#pragma once

#include "../Core/NGameManager.h"

class IFrameWork
{
public:
    virtual ~IFrameWork() = default;

    // Entry point
    int Run();

    // Life cycle hooks
    virtual void Initialise() = 0;
    virtual void PollEvents();
    virtual void Update(float dt);
    virtual void Render();
    virtual void Shutdown() = 0;

protected:

    bool m_isRunning = true;
    NGameManager m_gameMgr;
};