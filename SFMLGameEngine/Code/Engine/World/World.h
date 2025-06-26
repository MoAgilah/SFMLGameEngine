#pragma once

#include "../../Game/Sprite.h"
#include "../../Game/Objects/Enemy.h"
#include "../../Game/Objects/Object.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <map>
#include <memory>

enum class Texts { Max };
enum class Sprites { Max };

class World
{
public:
	World();
	~World() = default;

	virtual void Update(float deltaTime);
	void Render(sf::RenderWindow& window);
	virtual void ResetLevel();

	void CheckIsInView();

	virtual void AddEnemies();
	virtual void AddGUI();
	virtual void AddObjects();
	virtual void AddForeGroundSprites();

	[[nodiscard]] Object* GetObjectByName(const std::string& name);
	[[nodiscard]] Enemy* GetEnemyByName(const std::string& name);

private:

	virtual void UpdateGUI(float deltaTime);
	virtual void RenderGUI(sf::RenderWindow& window);

protected:

	Sprite m_backgroundSpr;
	BoundingBox m_foregroundBox;
	std::vector<sf::Text> m_texts;
	std::vector<Sprite> m_sprites;
	std::map<std::string, std::shared_ptr<Enemy>> m_enemies;
	std::map<std::string, std::shared_ptr<Object>> m_objects;
};