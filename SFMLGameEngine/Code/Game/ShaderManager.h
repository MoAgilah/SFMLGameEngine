#pragma once

#include "../Utilities/ResourceLoader.h"

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager() = default;

	sf::Shader* GetShader(const std::string& name);

private:

	ResourceLoader<sf::Shader> m_loader;
};