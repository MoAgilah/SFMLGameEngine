#pragma once

#include <string>
#include <map>
#include <memory>
#include<optional>
#include <filesystem>
namespace fs = std::filesystem;


#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

template<typename T>
class ResourceLoader
{
public:
	ResourceLoader() = default;
	ResourceLoader(const std::string& FilePaths);
	~ResourceLoader() = default;

	T* GetResource(std::string name);
	void LoadResources(fs::path path);

private:

	std::map<std::string, std::unique_ptr<T>> m_resources;
};

template<typename T>
ResourceLoader<T>::ResourceLoader(const std::string& path)
{
	LoadResources(path);
}

template<typename T>
inline void ResourceLoader<T>::LoadResources(fs::path path)
{
	if (!fs::exists(path))
		fs::create_directory(path);

	for (const auto& entry : fs::directory_iterator(path))
	{
		auto filename = entry.path().filename().replace_extension().string();
		m_resources.emplace(filename, std::make_unique<T>());
		m_resources.find(filename)->second->loadFromFile(entry.path().string());
	}
}

namespace
{
	std::optional<sf::Shader::Type> ShaderTypeFromFileExtension(const std::string& fileExt)
	{
		if (fileExt == "vert")
		{
			return sf::Shader::Type::Vertex;
		}
		else if (fileExt == "frag")
		{
			return sf::Shader::Type::Fragment;
		}
		else if (fileExt == "geom")
		{
			return sf::Shader::Type::Geometry;
		}

		return std::nullopt;
	}
}

template<>
inline void ResourceLoader<sf::Shader>::LoadResources(fs::path path)
{
	if (!fs::exists(path))
		fs::create_directory(path);

	for (const auto& entry : fs::directory_iterator(path))
	{
		auto type = ShaderTypeFromFileExtension(entry.path().extension().string().substr(1));
		if (type)
		{
			auto filename = entry.path().filename().replace_extension().string();
			m_resources.emplace(filename, std::make_unique<sf::Shader>());
			m_resources.find(filename)->second->loadFromFile(entry.path().string(), *type);
		}
	}
}

template<>
inline void ResourceLoader<sf::Music>::LoadResources(fs::path path)
{
	if (!fs::exists(path))
		fs::create_directory(path);

	for (const auto& entry : fs::directory_iterator(path))
	{
		auto filename = entry.path().filename().replace_extension().string();
		m_resources.emplace(filename, std::make_unique<sf::Music>());
		m_resources.find(filename)->second->openFromFile(entry.path().string());
	}
}

template<typename T>
inline T* ResourceLoader<T>::GetResource(std::string name)
{
	if (auto search = m_resources.find(name); search != m_resources.end())
		return search->second.get();

	return nullptr;
}