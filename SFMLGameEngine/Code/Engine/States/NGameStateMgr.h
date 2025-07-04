#pragma once

#include "../Interfaces/INGameState.h"
#include <string>
#include <vector>

class NGameStateMgr
{
public:
	~NGameStateMgr();

	std::string_view GetStateName();

	void ChangeState(INGameState* state);
	void PushState(INGameState* state);
	void PopState();
	void ClearStates();

	void Pause();
	void Resume();

	void ProcessInputs();
	void Update(float deltaTime);
	void Render();

private:

	std::vector<INGameState*> m_vGameStates;
};
