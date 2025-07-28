#pragma once

#include "../../../Engine/Interfaces/IShader.h"
#include "../../../Utilities/Traits.h"
#include <SFML/Graphics.hpp>
#include <filesystem>

namespace fs = std::filesystem;

class SFShader : public IShader
{
public:
    bool LoadFromFile(const std::string& filepath) override
    {
        fs::path entry = fs::path(filepath);
        auto type = GetNativeShaderType(ShaderTypeFromExtension(entry.extension().string().substr(1)));
        if (!type)
            return false;

        return m_shader.loadFromFile(filepath, *type);
    }

    sf::Shader& GetNativeShader() { return m_shader; }

private:

    std::optional<sf::Shader::Type> GetNativeShaderType(std::optional<ShaderType> shaderType)
    {
        if (shaderType)
        {
            switch (shaderType.value())
            {
            case ShaderType::Vertex:
                return sf::Shader::Type::Vertex;
                break;
            case ShaderType::Fragment:
                return sf::Shader::Type::Fragment;
                break;
            case ShaderType::Geometry:
                return sf::Shader::Type::Geometry;
                break;
            }
        }

        return std::nullopt;
    }

    sf::Shader m_shader;
};

template<>
struct ShaderTrait<SFShader>
{
    using FontType = SFShader;

    static std::unique_ptr<IShader> Create()
    {
        return std::make_unique<SFShader>();
    }
};