#pragma once

#include "Timer.h"
#include "../../Game/Camera.h"
#include "../Collisions/CollisionManager.h"
#include "../Interfaces/ICamera.h"
#include "../Interfaces/IRenderer.h"
#include "../Input/InputManager.h"
#include "../Resources/FontManager.h"
#include "../Resources/ShaderManager.h"
#include "../Resources/SoundManager.h"
#include "../Resources/TextureManager.h"
#include "../World/World.h"
#include "../States/GameStateMgr.h"
#include <array>
#include <memory>

class NGameManager
{
public:
	NGameManager();
	~NGameManager();
	NGameManager(const NGameManager& obj) = delete;

	static NGameManager* Get() { return m_instance; }

	void CheckInView();
	void Update(float deltaTime);
	void Render();

	void ChangeWorld(World* world);
	void ChangeCollisionManager(CollisionManager* mgr);

	// Getters
	[[nodiscard]] Timer& GetTimer() noexcept { return m_timer; }
	[[nodiscard]] ICamera* GetCamera() noexcept { return m_camera.get(); }
	[[nodiscard]] InputManager& GetInputManager() noexcept { return m_inputManager; }
	[[nodiscard]] FontManager& GetFontMgr() noexcept { return m_fontManager; }
	[[nodiscard]] ShaderManager& GetShaderMgr() noexcept { return m_shaderManager; }
	[[nodiscard]] SoundManager& GetSoundMgr() noexcept { return m_soundManager; }
	[[nodiscard]] TextureManager& GetTextureMgr() noexcept { return m_texureManager; }
	[[nodiscard]] CollisionManager* GetCollisionMgr() noexcept { return m_collisionManager.get(); }
	[[nodiscard]] GameStateMgr* GetGameStateMgr() noexcept { return &m_stateManager; }
	[[nodiscard]] IRenderer* GetRenderer() noexcept { return m_renderer.get(); }
	[[nodiscard]] World* GetWorld() { return m_world.get(); }

	// Setters
	void SetCamera(std::unique_ptr<ICamera> camera) { m_camera = std::move(camera); }
	void SetRenderer(std::unique_ptr<IRenderer> renderer) { m_renderer = std::move(renderer); }

private:
	static NGameManager* m_instance;

	Timer								m_timer;
	InputManager						m_inputManager;
	FontManager							m_fontManager;
	ShaderManager						m_shaderManager;
	SoundManager						m_soundManager;
	TextureManager						m_texureManager;
	GameStateMgr						m_stateManager;

	std::unique_ptr<ICamera>			m_camera;
	std::unique_ptr<IRenderer>			m_renderer;
	std::unique_ptr<CollisionManager>	m_collisionManager;
	std::unique_ptr<World>				m_world;
};
