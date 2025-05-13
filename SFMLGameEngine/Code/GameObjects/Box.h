#pragma once

#include "Object.h"
#include <memory>
#include <vector>

class Box : public Object
{
public:
	Box(const std::string& texID, const Point& boxSize, const Point& initPos);
	Box(const std::string& texID, const AnimationData& animData, const Point& boxSize, const Point& initPos);
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