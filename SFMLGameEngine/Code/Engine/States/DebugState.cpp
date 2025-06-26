#include "DebugState.h"

#include "../Core/Constants.h"

DebugState::DebugState(GameManager* gameMgr)
	: IGameState(gameMgr)
{}

void DebugState::Initialise()
{
	auto world = m_gameMgr->GetWorld();
	world->AddObjects();
	world->AddEnemies();
	world->AddForeGroundSprites();
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
	m_gameMgr->GetWorld()->Update(deltaTime);
}

void DebugState::Render()
{
	sf::RenderWindow& window = m_gameMgr->GetRenderWindow();

	m_gameMgr->GetCamera().Reset(window);
	m_gameMgr->GetWorld()->Render(window);

#if defined _DEBUG
	m_gameMgr->GetCollisionMgr()->Render(window);
#endif
}
