#pragma once

#include "GameState.h"
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class GameStateMgr
{
public:
	~GameStateMgr();

	std::string_view GetStateName();

	void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();
	void ClearStates();

	void Pause();
	void Resume();

	void ProcessInputs();
	void Update(float deltaTime);
	void Render();

private:

	std::vector<GameState*> m_vGameStates;
};
