#pragma once

#include "Game/Constants.h"
#include "Game/GameManager.h"

class FrameWork
{
public:
	FrameWork(const std::string& windowTitle = GameConstants::WindowTitle);
	~FrameWork() = default;

	void SetInitialGameState(GameState* state);

	int Run();

private:

	GameManager m_gameMgr;
};