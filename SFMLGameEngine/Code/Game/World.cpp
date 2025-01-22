#include "World.h"
#include "GameManager.h"
#include "../Collisions/BoundingVolume.h"
#include "../Game/Constants.h"
#include "../Game/Camera.h"

World::World()
{
	AddGUI();

	m_foregroundBox.Reset(sf::Vector2f(32, 47));
}


void World::Update(float deltaTime)
{
	for (const auto& enemy : m_enemies)
	{
		if (!enemy->GetActive())
			continue;

		enemy->Update(deltaTime);
	}

	for (const auto& object : m_objects)
	{
		if (!object->GetActive())
			continue;

		object->Update(deltaTime);
	}

	UpdateGUI();
}

void World::Render(sf::RenderWindow& window)
{
	m_backgroundSpr.Render(window);

	for (const auto& enemy : m_enemies)
	{
		if (!enemy->GetActive())
			continue;

		enemy->Render(window);
	}

	for (const auto& object : m_objects)
	{
		if (!object->GetActive())
			continue;

		object->Render(window);
	}
}

void World::ResetLevel()
{
	for (auto& enemy : m_enemies)
		enemy->Reset();

	for (auto& object : m_objects)
		object->Reset();
}

void World::CheckIsInView()
{
	auto& camera = GameManager::Get()->GetCamera();

	for (auto& enemy : m_enemies)
		enemy->SetActive(camera.IsInView(enemy->GetBoundingBox()));

	for (auto& object : m_objects)
		object->SetActive(camera.IsInView(object->GetBoundingBox()));
}

void World::AddEnemies()
{
}

void World::AddObjects()
{
}

void World::AddForeGroundSprites()
{
}

void World::AddGUI()
{
}

void World::UpdateGUI()
{
	auto view = GameManager::Get()->GetCamera().GetView();
}
