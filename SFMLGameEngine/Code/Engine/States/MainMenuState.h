#pragma once

#include "../Core/GameManager.h"
#include "../Interfaces/IGameState.h"
#include "../UI/Menu.h"

class MainMenuState final : public IGameState
{
public:
	explicit MainMenuState(GameManager* gameMgr);

	std::string_view GetStateName() const override { return "Debug"; }

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render() override;

private:

	Menu m_menu;
	Sprite m_backgroundSpr;
};