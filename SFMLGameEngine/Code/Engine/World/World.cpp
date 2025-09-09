#include "World.h"

#include "../Core/Constants.h"
#include "../Core/GameManager.h"
#include "../Collisions/BoundingVolume.h"
#include "../../Game/Camera.h"

World::World()
{
	AddGUI();

	m_foregroundBox.Reset({ 32, 47 });
}

void World::Update(float deltaTime)
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

void World::Render(sf::RenderWindow& window)
{
	m_backgroundSpr.Render(window);

	for (const auto& [_, enemy] : m_enemies)
	{
		if (!enemy->GetActive())
			continue;

		//enemy->Render(window);
	}

	for (const auto& [_, object] : m_objects)
	{
		if (!object->GetActive())
			continue;

		//object->Render(window);
	}

	RenderGUI(window);
}

void World::ResetLevel()
{
	for (auto& [_, enemy] : m_enemies)
		enemy->Reset();

	for (auto& [_, object] : m_objects)
		object->Reset();
}

void World::CheckIsInView()
{
	auto& camera = GameManager::Get()->GetCamera();

	/*for (auto& [_, enemy] : m_enemies)
		enemy->SetActive(camera.IsInView(enemy->GetBoundingBox()));*/

	/*for (auto& [_, object] : m_objects)
		object->SetActive(camera.IsInView(object->GetBoundingBox()));*/
}

void World::AddEnemies()
{
}

void World::AddGUI()
{
}

void World::AddObjects()
{
}

void World::AddForeGroundSprites()
{
}

Object* World::GetObjectByName(const std::string& name)
{
	auto it = m_objects.find(name);

	if (it != m_objects.end())
		return it->second.get();

	return nullptr;
}

Enemy* World::GetEnemyByName(const std::string& name)
{
	auto it = m_enemies.find(name);

	if (it != m_enemies.end())
		return it->second.get();

	return nullptr;
}

void World::UpdateGUI(float deltaTime)
{
	auto view = GameManager::Get()->GetCamera().GetView();
}

void World::RenderGUI(sf::RenderWindow& window)
{
}