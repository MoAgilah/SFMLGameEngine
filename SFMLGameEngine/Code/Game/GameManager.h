#pragma once

#include <array>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Timer.h"
#include "Camera.h"
#include "InputManager.h"
#include "FontManager.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "World.h"
#include "../Collisions/CollisionManager.h"
#include "../GameStates/GameStateMgr.h"

class GameManager
{
public:
	GameManager();
	~GameManager();
	GameManager(const GameManager& obj) = delete;

	static GameManager* Get() { return m_instance; }

	void CheckInView();

	sf::RenderWindow& GetRenderWindow() { return m_window; }

	void Update(float deltaTime);
	void Render();

	void ChangeWorld(World* world);
	World* GetWorld() { return m_world.get(); }

	Timer& GetTimer() { return m_timer; }
	Camera& GetCamera() { return m_camera; }
	InputManager& GetInputManager() { return m_inputManager; }
	FontManager& GetFontMgr() { return m_fontManager; }
	ShaderManager& GetShaderMgr() { return m_shaderManager; }
	TextureManager& GetTextureMgr() { return m_texureManager; }
	CollisionManager* GetCollisionMgr() { return m_collisionManager.get(); }
	GameStateMgr* GetGameStateMgr() { return &m_stateManager; }

private:

	static GameManager*					m_instance;
	sf::RenderWindow					m_window;
	Timer								m_timer;
	Camera								m_camera;
	InputManager						m_inputManager;
	FontManager							m_fontManager;
	ShaderManager						m_shaderManager;
	TextureManager						m_texureManager;
	GameStateMgr						m_stateManager;
	std::unique_ptr<World>				m_world;
	std::unique_ptr<CollisionManager>	m_collisionManager;
};