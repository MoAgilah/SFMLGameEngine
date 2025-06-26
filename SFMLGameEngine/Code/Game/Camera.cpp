#include "Camera.h"

#include "../Engine/Core/GameManager.h"
#include "../Engine/Core/Constants.h"

Camera::Camera()
{
	const auto& screenDim = GameConstants::ScreenDim;
	const sf::Vector2f center = screenDim * 0.5f;

	m_camera.setSize(screenDim);
	m_camera.setCenter(center);
	m_camera.setViewport({ {0.f, 0.f},{ 1.f, 1.f} });

	m_viewBox.Reset(screenDim);
	m_viewBox.Update(center);
	m_viewBox.SetFillColour(sf::Color(255, 0, 0, 128));
}

void Camera::Update()
{
	// Assuming dynamic camera follow logic will be added later
	const float posX = 0.f;

	m_camera.setCenter({ posX + (GameConstants::ScreenDim.x * 0.5f), m_camera.getCenter().y });
	m_viewBox.Update(m_camera.getCenter());
}

void Camera::Reset(sf::RenderWindow& window)
{
	Update(); // Ensure position is up-to-date
	window.setView(m_camera);
}

bool Camera::IsInView(BoundingVolume* volume)
{
	return m_viewBox.Intersects(volume);
}

bool Camera::CheckVerticalBounds(BoundingBox* box)
{
	const float cameraBottom = m_camera.getCenter().y + (GameConstants::ScreenDim.y * 0.5f);
	return box->GetPosition().y > (cameraBottom - (box->GetExtents().y * 2.f));
}

void Camera::RenderViewBox(sf::RenderWindow& window)
{
	m_viewBox.Render(window);
}