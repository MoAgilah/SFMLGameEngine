#include "MainMenuState.h"
#include "../Game/Constants.h"
#include "../GameStates/DebugState.h"

enum MenuPosition { Automation, Manual };

void MainMenuActionFunc(int menuPosition)
{
	GameManager::Get()->GetGameStateMgr()->ChangeState(new DebugState(GameManager::Get()));
}
//TextConfig(const std::string fontName, unsigned int charSize, const Point& position, TextAnimType textAnimType, sf::Color colour = sf::Color::Black, TextAlignment alignment = TextAlignment::None)

//TextBasedMenu(std::function<void(int)> func, const std::string& text, const TextConfig& config, unsigned int marginSize, std::optional<sf::Color> passiveColour = std::nullopt);
MainMenuState::MainMenuState(GameManager* gameMgr)
	: GameState("MainMenu"),
	m_menu(&MainMenuActionFunc, "Start",
		{ "Standard" ,30, { GameConstants::ScreenDim / 2.0f }, TextAnimType::Flashing }, 20)
{
	m_gameMgr = gameMgr;
}

void MainMenuState::Initialise()
{
	m_backgroundSpr.SetTexture("");
	m_backgroundSpr.SetScale(GameConstants::Scale);
	m_backgroundSpr.SetOrigin(Point());

	m_menu.Start();
}

void MainMenuState::Pause()
{
}

void MainMenuState::Resume()
{
}

void MainMenuState::ProcessInputs()
{
	m_menu.ProcessInput();
}

void MainMenuState::Update(float deltaTime)
{
	ProcessInputs();

	m_menu.Update(deltaTime);
}

void MainMenuState::Render()
{
	sf::RenderWindow& window = m_gameMgr->GetRenderWindow();

	m_backgroundSpr.Render(window);

	m_menu.Render(window);
}
