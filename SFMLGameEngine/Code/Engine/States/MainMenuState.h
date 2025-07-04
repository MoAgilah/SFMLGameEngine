#pragma once

#include "../Core/NGameManager.h"
#include "../Interfaces/INGameState.h"
#include "../UI/Menu.h"

class MainMenuState final : public INGameState
{
public:
	explicit MainMenuState(NGameManager* gameMgr);

	std::string_view GetStateName() const override { return "Debug"; }

	void Initialise() override;
	void Pause() override;
	void Resume() override;
	void ProcessInputs() override;
	void Update(float deltaTime) override;
	void Render() override;

private:

	Menu m_menu;
	SFSprite m_backgroundSpr;
};