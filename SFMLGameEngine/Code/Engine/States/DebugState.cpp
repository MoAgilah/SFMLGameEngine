#include "DebugState.h"

#include "../Core/Constants.h"

DebugState::DebugState(GameManager* gameMgr)
	: IGameState(gameMgr)
{}

void DebugState::Initialise()
{
	auto scene = m_gameMgr->GetScene();

	scene->AddObjects();
	scene->AddEnemies();
	scene->AddForeGroundSprites();
}

void DebugState::Pause()
{
}

void DebugState::Resume()
{
}

void DebugState::ProcessInputs()
{
}

void DebugState::Update(float deltaTime)
{
	m_gameMgr->GetTimer().Update(deltaTime);

	m_gameMgr->CheckInView();
	m_gameMgr->GetScene()->Update(deltaTime);
}

void DebugState::Render()
{
	sf::RenderWindow& window = m_gameMgr->GetRenderWindow();

	m_gameMgr->GetCamera().Reset(window);
	m_gameMgr->GetScene()->Render(window);

#if defined _DEBUG
	m_gameMgr->GetCollisionMgr()->Render(window);
#endif
}
