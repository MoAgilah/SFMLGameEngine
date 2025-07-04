#include "MainMenuState.h"

#include "../Core/Constants.h"
#include "../States/DebugState.h"

enum MenuPosition { Automation, Manual };

MainMenuState::MainMenuState(NGameManager* gameMgr)
	: INGameState(gameMgr),
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
	NTextConfig textConfig;
	textConfig.m_fontName = "Standard";
	textConfig.m_animType = NTextAnimType::Flashing;
	textConfig.m_alignment = NTextAlignment::Center;
	textConfig.m_colour = sf::Color::Black;

	auto cellSize = m_menu.GetCellSize();

	auto cell = m_menu.GetCell({ 0, 0 });
	textConfig.m_charSize = (int)(cellSize.y * 0.25f);
	textConfig.m_position = cell->GetPosition();

	auto text = cell->AddTextElement(new SFAnimatedText(textConfig));
	InitFlashingText(dynamic_cast<SFAnimatedText*>(text), "Start");
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
	auto renderer = m_gameMgr->GetRenderer();

	m_backgroundSpr.Render(renderer);

	m_menu.Render(renderer);
}
