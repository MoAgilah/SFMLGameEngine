#pragma once

#include <SFML/Window/event.hpp>
#include <array>
#include <chrono>

class InputManager
{
public:
	InputManager();
	~InputManager() = default;

	void ProcessKeyPressedEvent(sf::Event& event);
	void ProcessKeyReleasedEvent(sf::Event& event);

	bool GetKeyState(int key) { return m_keyStates[key]; }
	void SetKeyState(int key, bool val) { m_keyStates[key] = val; }

	bool IsAnyKeyPressed();

	std::chrono::steady_clock::time_point GetKeyPressTimestamp(sf::Keyboard::Key key) const
	{
		return m_keyPressTimestamps[key];
	}

	sf::Keyboard::Key GetFirstPressedKey(const std::vector<sf::Keyboard::Key>& keys) const;

private:

	void SetKeyPressed(sf::Keyboard::Key key);
	void SetKeyReleased(sf::Keyboard::Key key);
	void SetKeyState(sf::Keyboard::Key key, bool state) { m_keyStates[key] = state; }

	std::array<bool, sf::Keyboard::Key::KeyCount> m_keyStates;
	std::array<std::chrono::steady_clock::time_point, sf::Keyboard::Key::KeyCount> m_keyPressTimestamps;
};
