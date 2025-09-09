#pragma once

#include "Timer.h"
#include "../Collisions/NCollisionManager.h"
#include "../Interfaces/ICamera.h"
#include "../Interfaces/IRenderer.h"
#include "../Interfaces/IScene.h"
#include "../Input/InputManager.h"
#include "../Resources/FontManager.h"
#include "../Resources/ShaderManager.h"
#include "../Resources/SoundManager.h"
#include "../Resources/TextureManager.h"
#include "../States/NGameStateMgr.h"
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

	// Getters
	[[nodiscard]] Timer& GetTimer() noexcept { return m_timer; }
	[[nodiscard]] ICamera* GetCamera() noexcept { return m_camera.get(); }
	[[nodiscard]] InputManager* GetInputManager() noexcept { return m_inputManager.get(); }
	[[nodiscard]] FontManager& GetFontMgr() noexcept { return m_fontManager; }
	[[nodiscard]] ShaderManager& GetShaderMgr() noexcept { return m_shaderManager; }
	[[nodiscard]] SoundManager& GetSoundMgr() noexcept { return m_soundManager; }
	[[nodiscard]] TextureManager& GetTextureMgr() noexcept { return m_texureManager; }
	[[nodiscard]] NCollisionManager* GetCollisionMgr() noexcept { return m_collisionManager.get(); }
	[[nodiscard]] NGameStateMgr* GetGameStateMgr() noexcept { return &m_stateManager; }
	[[nodiscard]] IRenderer* GetRenderer() noexcept { return m_renderer.get(); }
	[[nodiscard]] IScene* GetScene() { return m_scene.get(); }

	void InitInputManager(INativeKeyConverter* converter) { m_inputManager = std::make_shared<InputManager>(converter); }

	// Setters
	void SetCollisionManager(std::shared_ptr<NCollisionManager> colMgr) { m_collisionManager = std::move(colMgr); }
	void SetScene(std::shared_ptr<IScene> scene) { m_scene = std::move(scene); }
	void SetCamera(std::shared_ptr<ICamera> camera) { m_camera = std::move(camera); }
	void SetRenderer(std::shared_ptr<IRenderer> renderer) { m_renderer = std::move(renderer); }

private:
	static NGameManager* m_instance;

	Timer								m_timer;
	FontManager							m_fontManager;
	ShaderManager						m_shaderManager;
	SoundManager						m_soundManager;
	TextureManager						m_texureManager;
	NGameStateMgr						m_stateManager;

	std::shared_ptr<ICamera>			m_camera;
	std::shared_ptr<InputManager>		m_inputManager;
	std::shared_ptr<IRenderer>			m_renderer;
	std::shared_ptr<NCollisionManager>	m_collisionManager;
	std::shared_ptr<IScene>				m_scene;
};
