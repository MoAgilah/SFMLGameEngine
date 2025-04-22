#pragma once

#include "GameState.h"
#include "../Game/GameManager.h"
#include "../UIElements/Menu.h"

class MainMenuState : public GameState
{
public:
	explicit MainMenuState(GameManager* gameMgr);
	~MainMenuState() override = default;

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render() override;

private:

	TextBasedMenu m_menu;
	Sprite m_backgroundSpr;
	GameManager* m_gameMgr;
};