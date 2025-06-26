#pragma once

#include "../Core/GameManager.h"
#include "../Interfaces/IGameState.h"

class DebugState final : public IGameState
{
public:
	explicit DebugState(GameManager* gameMgr);

	std::string_view GetStateName() const override { return "Debug"; }

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render() override;
};