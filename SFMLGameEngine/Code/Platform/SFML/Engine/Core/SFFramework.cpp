#include "SFFramework.h"

#include "../../Renderer/SFCamera.h"
#include "../../Renderer/SFRenderer.h"

SFFramework::SFFramework()
{
	m_gameMgr.SetCamera(std::make_unique<SFCamera>());
	m_gameMgr.SetRenderer(std::make_unique<SFRenderer>());
}