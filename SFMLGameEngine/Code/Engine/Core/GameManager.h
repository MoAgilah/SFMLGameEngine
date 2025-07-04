#pragma once

#include "Timer.h"
#include "../../Game/Camera.h"
#include "../Collisions/CollisionManager.h"
#include "../Interfaces/IRenderer.h"
#include "../Input/InputManager.h"
#include "../Resources/FontManager.h"
#include "../Resources/ShaderManager.h"
#include "../Resources/SoundManager.h"
#include "../Resources/TextureManager.h"
#include "../World/World.h"
#include "../States/GameStateMgr.h"
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

	void ChangeCollisionManager(CollisionManager* mgr);

	[[nodiscard]] Timer& GetTimer() noexcept { return m_timer; }
	[[nodiscard]] Camera& GetCamera() noexcept { return m_camera; }
	[[nodiscard]] InputManager* GetInputManager() noexcept { return m_inputManager.get(); }
	[[nodiscard]] FontManager& GetFontMgr() noexcept { return m_fontManager; }
	[[nodiscard]] ShaderManager& GetShaderMgr() noexcept { return m_shaderManager; }
	[[nodiscard]] SoundManager& GetSoundMgr() noexcept { return m_soundManager; }
	[[nodiscard]] TextureManager& GetTextureMgr() noexcept { return m_texureManager; }
	[[nodiscard]] CollisionManager* GetCollisionMgr() noexcept { return m_collisionManager.get(); }
	[[nodiscard]] GameStateMgr* GetGameStateMgr() noexcept { return &m_stateManager; }
	[[nodiscard]] IRenderer* GetRenderer() noexcept { return m_renderer.get(); }
	[[nodiscard]] World* GetWorld() { return m_world.get(); }

private:

	static GameManager*					m_instance;

	sf::RenderWindow					m_window;
	Timer								m_timer;
	Camera								m_camera;
	std::unique_ptr<InputManager>						m_inputManager;
	FontManager							m_fontManager;
	ShaderManager						m_shaderManager;
	SoundManager						m_soundManager;
	TextureManager						m_texureManager;
	GameStateMgr						m_stateManager;
	std::unique_ptr<IRenderer>			m_renderer;
	std::unique_ptr<CollisionManager>	m_collisionManager;
	std::unique_ptr<World>				m_world;
};