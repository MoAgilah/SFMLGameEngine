#include "../Game/GameManager.h"
#include "../Game/Constants.h"
#include "../GameStates/DebugState.h"

GameManager* GameManager::m_instance = nullptr;

GameManager::GameManager()
	: m_timer(300.f)
{
	m_instance = this;
	m_collisionManager = std::make_unique<CollisionManager>();

	m_world = std::make_unique<World>();

	m_stateManager.ChangeState(new DebugState(this));
}

GameManager::~GameManager()
{
	if (m_instance)
		m_instance = nullptr;
}

void GameManager::CheckInView()
{
	for (auto& tile : m_collisionManager->GetGrid())
		tile->SetActive(m_camera.IsInView(tile->GetBoundingBox()));

	m_world->CheckIsInView();
}

void GameManager::Update(float deltaTime)
{
	m_stateManager.Update(deltaTime);
}

void GameManager::Render()
{
	m_stateManager.Render();
}

void GameManager::ChangeWorld(World* world)
{
	m_world.reset(world);
}

void GameManager::ChangeCollisionManager(CollisionManager* mgr)
{
	m_collisionManager.reset(mgr);
}
