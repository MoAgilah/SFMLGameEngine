#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class GameState
{
public:
	explicit GameState(std::string_view name)
		: m_StateName(name) {
	}

	std::string_view GetStateName() const { return m_StateName; }
	virtual ~GameState() = default;

	virtual void Initialise() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;
	virtual void ProcessInputs() = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;

private:

	std::string m_StateName;
};