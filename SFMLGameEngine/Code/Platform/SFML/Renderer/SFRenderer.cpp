#include "SFRenderer.h"

#include "../../../Engine/Core/Constants.h"
#include "../../../Engine/Core/GameManager.h"
#include "../../../Engine/Interfaces/IRenderable.h"

void SFRenderer::Initialise(const Point& screenDims, const std::string& title)
{
    m_window->Create(screenDims, title);
}

void SFRenderer::PollWindowEvents()
{
	m_window->PollEvents();
}

void SFRenderer::Clear()
{
}

void SFRenderer::Draw(IRenderable* object)
{
}

void SFRenderer::Present()
{
}