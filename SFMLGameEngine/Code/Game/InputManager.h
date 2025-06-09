#pragma once

#include <SFML/Window/event.hpp>
#include <array>
#include <chrono>

class InputManager
{
public:
	InputManager();
	~InputManager() = default;

	void ProcessKeyPressedEvent(const sf::Event::KeyPressed* event);
	void ProcessKeyReleasedEvent(const sf::Event::KeyReleased* event);

	bool GetKeyState(int key) { return m_keyStates[key]; }

	bool IsAnyKeyPressed();

	std::chrono::steady_clock::time_point GetKeyPressTimestamp(sf::Keyboard::Key key) const
	{
		return m_keyPressTimestamps[static_cast<int>(key)];
	}

	sf::Keyboard::Key GetFirstPressedKey(const std::vector<sf::Keyboard::Key>& keys) const;

private:

	void SetKeyPressed(sf::Keyboard::Key key);
	void SetKeyReleased(sf::Keyboard::Key key);
	void SetKeyState(sf::Keyboard::Key key, bool state) { m_keyStates[static_cast<int>(key)] = state; }

	std::array<bool, sf::Keyboard::KeyCount> m_keyStates;
	std::array<std::chrono::steady_clock::time_point, sf::Keyboard::KeyCount> m_keyPressTimestamps;
};
