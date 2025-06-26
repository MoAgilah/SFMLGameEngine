#pragma once

#include <SFML/Window/Keyboard.hpp>
#include <vector>

class MenuNav
{
public:
	MenuNav(sf::Keyboard::Key ascKey, sf::Keyboard::Key desKey);
	~MenuNav() = default;

	void HandleNavigation();

	void ChangeNavKeys(sf::Keyboard::Key ascKey, sf::Keyboard::Key desKey);

	void SetCursorRange(const std::vector<int>& cursorRange);

	int GetCurrCursorPos() const { return m_currCursorPos; }
	void SetCurrCursorPos(int currPos);
	void SetPrevCursorPos(int prevPos) { m_prevCursorPos = prevPos; }

	bool HasMoved() const { return m_currCursorPos != m_prevCursorPos; }

private:

	void HandleDirection(bool isPressed, bool& canMove, int direction);

	bool m_canAsc = false;
	bool m_canDes = true;
	int m_currCursorPos = 0;
	int m_prevCursorPos = -1;
	sf::Keyboard::Key m_ascKey;
	sf::Keyboard::Key m_desKey;
	std::vector<int> m_cursorRange;
};