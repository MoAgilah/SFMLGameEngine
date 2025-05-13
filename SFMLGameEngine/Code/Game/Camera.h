#pragma once

#include "../Collisions/BoundingVolume.h"
#include <SFML/Graphics.hpp>

class Camera
{
public:
	Camera();
	~Camera() = default;

	void Update();

	sf::View GetView() const { return m_camera; }

	bool IsInView(BoundingVolume* box);
	bool CheckVerticalBounds(BoundingBox* box);
	void Reset(sf::RenderWindow& window);
	void RenderViewBox(sf::RenderWindow& window);

private:

	sf::View m_camera;
	BoundingBox m_viewBox;
};