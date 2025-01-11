#include "ShaderManager.h"
#include "../Game/Constants.h"

ShaderManager::ShaderManager()
	: m_loader(GameConstants::ShaderFilePaths)
{
}

sf::Shader* ShaderManager::GetShader(const std::string& name)
{
	return m_loader.GetResource(name);
}
