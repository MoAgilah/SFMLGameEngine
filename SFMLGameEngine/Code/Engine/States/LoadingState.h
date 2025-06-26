#pragma once

#include "../Core/GameManager.h"
#include "../Interfaces/IGameState.h"
#include "../UI/Text.h"

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