#pragma once

#include "GameState.h"
#include "../Game/GameManager.h"

class DebugState : public GameState
{
public:
	explicit DebugState(GameManager* gameMgr);
	~DebugState() override = default;

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render() override;

private:

	GameManager* m_gameMgr;
};