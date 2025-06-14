#pragma once

#include "SFMLWindow.h"
#include "../Interfaces/IRenderer.h"

class SFMLRenderer : public IRenderer
{
public:
    void Initialise(const Point& screenDims, const std::string& title) override;
	void PollWindowEvents() override;
	void Clear() override;
	void Draw(IRenderable* object) override;
	void Present() override;
};