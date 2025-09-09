#include "NGameManager.h"

#include "Constants.h"
#include "../Interfaces/ITile.h"
#include "../States/DebugState.h"

NGameManager* NGameManager::m_instance = nullptr;

NGameManager::NGameManager()
	: m_timer(300.f)
{
	m_instance = this;
}

NGameManager::~NGameManager()
{
	if (m_instance)
		m_instance = nullptr;
}

void NGameManager::CheckInView()
{
	for (auto& tile : m_collisionManager->GetGrid())
		tile->SetActive(m_camera->IsInView(tile->GetBoundingBox()));

	m_scene->CheckIsInView();
}

void NGameManager::Update(float deltaTime)
{
	m_stateManager.Update(deltaTime);
}

void NGameManager::Render()
{
	m_stateManager.Render();
}