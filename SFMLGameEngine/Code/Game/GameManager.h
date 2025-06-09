#pragma once

#include "Camera.h"
#include "InputManager.h"
#include "FontManager.h"
#include "ShaderManager.h"
#include "SoundManager.h"
#include "Timer.h"
#include "TextureManager.h"
#include "World.h"
#include "../Collisions/CollisionManager.h"
#include "../GameStates/GameStateMgr.h"
#include <SFML/Graphics.hpp>
#include <array>
#include <memory>

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
	[[nodiscard]] World* GetWorld() { return m_world.get(); }

	void ChangeCollisionManager(CollisionManager* mgr);

	[[nodiscard]] Timer& GetTimer() noexcept { return m_timer; }
	[[nodiscard]] Camera& GetCamera() noexcept { return m_camera; }
	[[nodiscard]] InputManager& GetInputManager() noexcept { return m_inputManager; }
	[[nodiscard]] FontManager& GetFontMgr() noexcept { return m_fontManager; }
	[[nodiscard]] ShaderManager& GetShaderMgr() noexcept { return m_shaderManager; }
	[[nodiscard]] SoundManager& GetSoundMgr() noexcept { return m_soundManager; }
	[[nodiscard]] TextureManager& GetTextureMgr() noexcept { return m_texureManager; }
	[[nodiscard]] CollisionManager* GetCollisionMgr() noexcept { return m_collisionManager.get(); }
	[[nodiscard]] GameStateMgr* GetGameStateMgr() noexcept { return &m_stateManager; }

private:

	static GameManager*					m_instance;

	sf::RenderWindow					m_window;
	Timer								m_timer;
	Camera								m_camera;
	InputManager						m_inputManager;
	FontManager							m_fontManager;
	ShaderManager						m_shaderManager;
	SoundManager						m_soundManager;
	TextureManager						m_texureManager;
	GameStateMgr						m_stateManager;
	std::unique_ptr<World>				m_world;
	std::unique_ptr<CollisionManager>	m_collisionManager;
};