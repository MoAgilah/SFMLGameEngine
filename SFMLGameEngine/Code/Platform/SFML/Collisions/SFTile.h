#pragma once

#include "../../../Engine/Interfaces/ITile.h"
#include <SFML/Graphics.hpp>

class SFTile : public ITile
{
public:
	SFTile();
	SFTile(int gX, int gY, sf::Font* font);
	~SFTile() = default;

	void Render(IRenderer* renderer) override;
	void ResolveCollision(IDynamicGameObject* obj) override;

	void SetFillColour(Colour col) override;
	void SetOutlineColour(Colour col) override;

protected:

	Point GetSeperationVector(IDynamicGameObject* obj) { return m_aabb->GetSeparationVector(obj->GetVolume()); }

	void ResolveObjectToBoxTop(IDynamicGameObject* obj) override;
	void ResolveObjectToBoxBottom(IDynamicGameObject* obj) override;
	void ResolveObjectToBoxHorizontally(IDynamicGameObject* obj) override;
	bool ResolveObjectToSlopeTop(IDynamicGameObject* obj) override;
	bool ResolveObjectToSlopeIncline(IDynamicGameObject* obj, int start, int end) override;
	bool ResolveObjectToSlopeDecline(IDynamicGameObject* obj, int start, int end) override;
	void ResolveObjectToEdgeBounds(IDynamicGameObject* obj) override;
};