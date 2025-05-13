#include "InputManager.h"

#include <iostream>
#include <chrono>

InputManager::InputManager()
{
	m_keyStates.fill(false);
	m_keyPressTimestamps.fill(std::chrono::steady_clock::time_point::min());
}

void InputManager::ProcessKeyPressedEvent(sf::Event& event)
{
	SetKeyPressed(event.key.code);
}

void InputManager::ProcessKeyReleasedEvent(sf::Event& event)
{
	SetKeyReleased(event.key.code);
}

sf::Keyboard::Key InputManager::GetFirstPressedKey(const std::vector<sf::Keyboard::Key>& keys) const
{
	sf::Keyboard::Key firstKey = sf::Keyboard::Unknown;
	auto earliestTime = std::chrono::steady_clock::time_point::max();

	for (auto key : keys)
	{
		auto time = m_keyPressTimestamps[key];
		if (time != std::chrono::steady_clock::time_point::min() && time < earliestTime)
		{
			earliestTime = time;
			firstKey = key;
		}
	}

	return firstKey;
}

void InputManager::SetKeyPressed(sf::Keyboard::Key key)
{
	if (!m_keyStates[key]) // Only record the timestamp on first press
		m_keyPressTimestamps[key] = std::chrono::steady_clock::now();

	SetKeyState(key, true);
}

void InputManager::SetKeyReleased(sf::Keyboard::Key key)
{
	SetKeyState(key, false);
	m_keyPressTimestamps[key] = std::chrono::steady_clock::time_point::min(); // Reset timestamp on release
}

bool InputManager::IsAnyKeyPressed()
{
	return std::any_of(m_keyStates.begin(), m_keyStates.end(), [](const auto& keyState) { return keyState == true; });
}