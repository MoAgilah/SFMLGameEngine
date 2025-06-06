#include "MenuNavigation.h"

#include "Game/GameManager.h"

MenuNav::MenuNav(sf::Keyboard::Key ascKey, sf::Keyboard::Key desKey)
	: m_ascKey(ascKey), m_desKey(desKey)
{
}

void MenuNav::HandleNavigation()
{
	auto& inputManager = GameManager::Get()->GetInputManager();

	HandleDirection(inputManager.GetKeyState((int)m_ascKey), m_canAsc, -1);
	HandleDirection(inputManager.GetKeyState((int)m_desKey), m_canDes, 1);
}

void MenuNav::ChangeNavKeys(sf::Keyboard::Key ascKey, sf::Keyboard::Key desKey)
{
	m_ascKey = ascKey;
	m_desKey = desKey;
}

void MenuNav::SetCursorRange(const std::vector<int>& cursorRange)
{
	m_cursorRange = cursorRange;
}

void MenuNav::SetCurrCursorPos(int currPos)
{
	if (currPos >= *(m_cursorRange.begin()) && currPos <= *(m_cursorRange.rbegin()))
	{
		m_currCursorPos = currPos;
	}
}

void MenuNav::HandleDirection(bool isPressed, bool& canMove, int direction)
{
	if (isPressed)
	{
		if (canMove)
		{
			SetCurrCursorPos(m_currCursorPos + direction);
			canMove = false;
		}
	}
	else
	{
		canMove = true;
	}
}