#include "IScene.h"

#include "../Core/Constants.h"
#include "../Core/NGameManager.h"

void IScene::Update(float deltaTime)
{
	UpdateGUI(deltaTime);

	for (const auto& [_, enemy] : m_enemies)
	{
		if (!enemy->GetActive())
			continue;

		enemy->Update(deltaTime);
	}

	for (const auto& [_, object] : m_objects)
	{
		if (!object->GetActive())
			continue;

		object->Update(deltaTime);
	}
}

void IScene::Render(IRenderer* renderer)
{
	m_backgroundSpr->Render(renderer);

	for (const auto& [_, enemy] : m_enemies)
	{
		if (!enemy->GetActive())
			continue;

		enemy->Render(renderer);
	}

	for (const auto& [_, object] : m_objects)
	{
		if (!object->GetActive())
			continue;

		object->Render(renderer);
	}

	RenderGUI(renderer);
}

void IScene::ResetScene()
{
	for (auto& [_, enemy] : m_enemies)
		enemy->Reset();

	for (auto& [_, object] : m_objects)
		object->Reset();
}

void IScene::CheckIsInView()
{
	auto camera = NGameManager::Get()->GetCamera();

	for (auto& [_, enemy] : m_enemies)
		enemy->SetActive(camera->IsInView(enemy->GetVolume()));

	for (auto& [_, object] : m_objects)
		object->SetActive(camera->IsInView(object->GetVolume()));
}

NGameObject* IScene::GetObjectByName(const std::string& name)
{
	auto it = m_objects.find(name);

	if (it != m_objects.end())
		return it->second.get();

	return nullptr;
}

Enemy* IScene::GetEnemyByName(const std::string& name)
{
	auto it = m_enemies.find(name);

	if (it != m_enemies.end())
		return it->second.get();

	return nullptr;
}