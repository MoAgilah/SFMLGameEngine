#include "SFMLRenderer.h"

#include "../Game/Constants.h"
#include "../Game/GameManager.h"
#include "../Interfaces/IRenderable.h"

void SFMLRenderer::Initialise(const Point& screenDims, const std::string& title)
{
    m_window->Create(screenDims, title);
}

void SFMLRenderer::PollWindowEvents()
{
	m_window->PollEvents();
}

void SFMLRenderer::Clear()
{
}

void SFMLRenderer::Draw(IRenderable* object)
{
}

void SFMLRenderer::Present()
{
}