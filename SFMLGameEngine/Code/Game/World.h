#pragma once

#include <memory>
#include <array>
#include <SFML/Graphics.hpp>
#include "../Drawables/Sprite.h"
#include "../GameObjects/Enemy.h"
#include "../GameObjects/Object.h"

enum class Texts { Max };
enum class Sprites { Max};

class World
{
public:
	World();
	~World() = default;

	virtual void Update(float deltaTime);
	void Render(sf::RenderWindow& window);
	void ResetLevel();

	void CheckIsInView();

	virtual void AddEnemies();
	virtual void AddGUI();
	virtual void AddObjects();
	virtual void AddForeGroundSprites();

private:

	virtual void UpdateGUI();
	virtual void RenderGUI(sf::RenderWindow& window);

protected:

	BoundingBox m_foregroundBox;
	Sprite m_backgroundSpr;
	std::vector<sf::Text> m_texts;
	std::vector<Sprite> m_sprites;
	std::vector<std::shared_ptr<Enemy>> m_enemies;
	std::vector<std::shared_ptr<Object>> m_objects;
};