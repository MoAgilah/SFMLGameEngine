#include "NGameStateMgr.h"

#include "../Core/Constants.h"
#include <iostream>
#include <format>

NGameStateMgr::~NGameStateMgr()
{
	ClearStates();
}

std::string_view NGameStateMgr::GetStateName()
{
	if (!m_vGameStates.empty())
		return m_vGameStates.back()->GetStateName();

	return std::string_view();
}

void NGameStateMgr::ChangeState(INGameState* state)
{
	if (!m_vGameStates.empty())
		m_vGameStates.pop_back();

	m_vGameStates.push_back(state);
	m_vGameStates.back()->Initialise();

	if (GameConstants::DRender)
		std::cout << std::format("Changed too {} state\n", m_vGameStates.back()->GetStateName());
}

void NGameStateMgr::PushState(INGameState* state)
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Pause();

	m_vGameStates.push_back(state);
	m_vGameStates.back()->Initialise();

	if (GameConstants::DRender)
		std::cout << std::format("Pushed {} state\n", m_vGameStates.back()->GetStateName());
}

void NGameStateMgr::PopState()
{
	if (!m_vGameStates.empty())
	{
		if (GameConstants::DRender)
			std::cout << std::format("Popped {} state\n", m_vGameStates.back()->GetStateName());
		m_vGameStates.pop_back();
	}

	if (!m_vGameStates.empty())
		m_vGameStates.back()->Resume();
}

void NGameStateMgr::ClearStates()
{
	while (!m_vGameStates.empty())
		m_vGameStates.pop_back();
}

void NGameStateMgr::Pause()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Pause();
}

void NGameStateMgr::Resume()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Resume();
}

void NGameStateMgr::ProcessInputs()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->ProcessInputs();
}

void NGameStateMgr::Update(float deltaTime)
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Update(deltaTime);
}

void NGameStateMgr::Render()
{
	if (!m_vGameStates.empty())
		m_vGameStates.back()->Render();
}
