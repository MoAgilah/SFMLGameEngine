#include "MainMenuState.h"
#include "../Game/Constants.h"
#include "../GameStates/DebugState.h"

enum MenuPosition { Automation, Manual };

void MainMenuActionFunc(int menuPosition)
{
	GameManager::Get()->GetGameStateMgr()->ChangeState(new DebugState(GameManager::Get()));
}

MainMenuState::MainMenuState(GameManager* gameMgr)
	: GameState("MainMenu"),
	m_menu(&MainMenuActionFunc,
		"Standard",
		TextAnimType::Flashing,
		"Start",
		30,
		20,
		{GameConstants::ScreenDim / 2.0f})
{
	m_gameMgr = gameMgr;
}

void MainMenuState::Initialise()
{
	m_backgroundSpr.SetTexture("");
	m_backgroundSpr.SetScale(GameConstants::Scale);
	m_backgroundSpr.SetOrigin(Point());
}

void MainMenuState::Pause()
{
}

void MainMenuState::Resume()
{
}

void MainMenuState::ProcessInputs()
{
	m_menu.ProcessInput();
}

void MainMenuState::Update(float deltaTime)
{
	ProcessInputs();

	m_menu.Update(deltaTime);
}

void MainMenuState::Render()
{
	sf::RenderWindow& window = m_gameMgr->GetRenderWindow();

	m_backgroundSpr.Render(window);

	m_menu.Render(window);
}
