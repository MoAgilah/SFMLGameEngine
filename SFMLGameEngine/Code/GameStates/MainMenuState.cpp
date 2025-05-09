#include "MainMenuState.h"
#include "../Game/Constants.h"
#include "../GameStates/DebugState.h"

enum MenuPosition { Automation, Manual };

MainMenuState::MainMenuState(GameManager* gameMgr)
	: GameState("MainMenu"),
	m_menu({ GameConstants::ScreenDim.x * 0.8f, GameConstants::ScreenDim.y * 0.4f }, 2.f, { 1,2 }, { MenuPositionMode::Anchored, GameConstants::ScreenDim })
{
	m_gameMgr = gameMgr;
}

void MainMenuState::Initialise()
{
	m_backgroundSpr.SetTexture("");
	m_backgroundSpr.SetScale(GameConstants::Scale);
	m_backgroundSpr.SetOrigin(Point());

	// Set up menu
	TextConfig textConfig;
	textConfig.m_fontName = "Standard";
	textConfig.m_animType = TextAnimType::Flashing;
	textConfig.m_alignment = TextAlignment::Center;
	textConfig.m_colour = sf::Color::Black;

	auto cellSize = m_menu.GetCellSize();

	auto cell = m_menu.GetCell({ 0, 0 });
	textConfig.m_charSize = (int)(cellSize.y * 0.25f);
	textConfig.m_position = cell->GetPosition();

	auto text = cell->AddTextElement(new AnimatedText(textConfig));
	dynamic_cast<AnimatedText*>(text)->InitFlashingText("Start");
	cell->SetMenuSlotNumber(0);

	m_menu.SetActiveCells();

	auto menuNav = m_menu.GetMenuNav();

	menuNav->SetCursorRange({ 0 });
	menuNav->SetCurrCursorPos(0);
}

void MainMenuState::Pause()
{
}

void MainMenuState::Resume()
{
}

void MainMenuState::ProcessInputs()
{
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
