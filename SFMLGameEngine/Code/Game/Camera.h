#pragma once

#include "../Collisions/BoundingVolume.h"
#include <SFML/Graphics.hpp>

class Camera
{
public:
	Camera();
	~Camera() = default;

	void Update();
	void Reset(sf::RenderWindow& window);
	void RenderViewBox(sf::RenderWindow& window);

	sf::View GetView() const { return m_camera; }

	bool IsInView(BoundingVolume* volume);
	bool CheckVerticalBounds(BoundingBox* box);

private:
	sf::View m_camera;
	BoundingBox m_viewBox;
};
