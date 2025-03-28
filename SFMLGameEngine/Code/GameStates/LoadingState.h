#pragma once

#include "GameState.h"
#include "../Game/GameManager.h"
#include "../UIElements/Menu.h"

void LoadResources();

class LoadingState : public GameState
{
public:
	explicit LoadingState(GameManager* gameMgr);
	~LoadingState() override = default;

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render() override;

private:

	Sprite m_backgroundSpr;
	Text m_loadingMessage;
	GameManager* m_gameMgr;
};