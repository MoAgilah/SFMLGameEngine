#pragma once

#include "../Interfaces/IGameState.h"
#include "../Game/GameManager.h"
#include "../UIElements/Text.h"

void LoadResources();

class LoadingState final : public IGameState
{
public:
	explicit LoadingState(GameManager* gameMgr);

	std::string_view GetStateName() const override { return "Loading"; }

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render() override;

private:

	Sprite m_backgroundSpr;
	AnimatedText m_loadingMessage;
};