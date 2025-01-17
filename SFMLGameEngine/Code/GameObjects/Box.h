#pragma once

#include <memory>
#include <vector>
#include "Object.h"

class Box : public Object
{
public:
	Box(const std::string& texID, const sf::Vector2f& boxSize, const sf::Vector2f& initPos);
	Box(const std::string& texID, const AnimationData& animData, const sf::Vector2f& boxSize, const sf::Vector2f& initPos);
	~Box() override = default;

	void Reset() override;

	void SetCanHit(bool canHit) { m_canHit = canHit; }
	bool GetCanHit() const { return m_canHit; }
	void SetJustHit(bool justHit) { m_justHit = justHit; }
	bool GetJustHit() const { return m_justHit; }

	void WasJustHit();

private:

	bool m_canHit = true;
	bool m_justHit = false;
};