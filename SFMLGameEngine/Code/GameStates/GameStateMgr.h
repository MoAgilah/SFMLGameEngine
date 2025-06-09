#pragma once

#include "IGameState.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class GameStateMgr
{
public:
	~GameStateMgr();

	std::string_view GetStateName();

	void ChangeState(IGameState* state);
	void PushState(IGameState* state);
	void PopState();
	void ClearStates();

	void Pause();
	void Resume();

	void ProcessInputs();
	void Update(float deltaTime);
	void Render();

private:

	std::vector<IGameState*> m_vGameStates;
};
