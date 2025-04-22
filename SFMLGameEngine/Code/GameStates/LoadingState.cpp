#include "LoadingState.h"
#include <thread>
#include "../Game/Constants.h"
#include "../GameStates/MainMenuState.h"

void LoadResources()
{
	GameConstants::GameIsReady = true;
}

LoadingState::LoadingState(GameManager* gameMgr)
	: GameState("Loading"), m_loadingMessage({"Standard", 30, { GameConstants::ScreenDim / 2.0f }, Flashing })
{
	m_gameMgr = gameMgr;
}

void LoadingState::Initialise()
{
	m_backgroundSpr.SetTexture("");
	m_backgroundSpr.SetScale(GameConstants::Scale);
	m_backgroundSpr.SetOrigin(Point());

	m_loadingMessage.SetText("Loading");

	std::thread t(LoadResources);
	t.detach();
}

void LoadingState::Pause()
{
}

void LoadingState::Resume()
{
}

void LoadingState::ProcessInputs()
{
}

void LoadingState::Update(float deltaTime)
{
	ProcessInputs();

	m_loadingMessage.Update(deltaTime);

	if (GameConstants::GameIsReady)
		m_gameMgr->GetGameStateMgr()->PopState();
}

void LoadingState::Render()
{
	sf::RenderWindow& window = m_gameMgr->GetRenderWindow();

	m_backgroundSpr.Render(window);
	m_loadingMessage.Render(window);
}
