#pragma once

#include "Game/Constants.h"
#include "Game/GameManager.h"

class FrameWork
{
public:
	FrameWork(const std::string& windowTitle = GameConstants::WindowTitle);
	~FrameWork() = default;

	virtual void Initialise();

	int Run();

protected:

	GameManager m_gameMgr;
};