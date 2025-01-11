#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include<optional>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

template<typename T>
class ResourceLoader
{
public:
	ResourceLoader(const std::string& FilePaths);
	~ResourceLoader() = default;

	void LoadResources(const std::vector<std::string>& keys, const std::vector<std::string>& paths);

	T* GetResource(std::string name);

private:


	std::map<std::string, std::unique_ptr<T>> m_resources;
};

template<typename T>
ResourceLoader<T>::ResourceLoader(const std::string& FilePaths)
{
	std::ifstream file(FilePaths);
	std::string line;
	std::vector<std::string> lines;

	if (file.is_open())
	{
		while (getline(file, line))
			lines.push_back(line);

		file.close();
	}

	std::vector<std::string> keys;
	std::vector<std::string> paths;
	for (auto& l : lines)
	{
		auto pos = l.find(" ");
		keys.push_back(l.substr(0, pos));
		paths.push_back(l.substr(pos + 1));
	}

	LoadResources(keys, paths);
}

template<typename T>
inline void ResourceLoader<T>::LoadResources(const std::vector<std::string>& keys, const std::vector<std::string>& paths)
{
	if (keys.size() != paths.size())
		return;

	std::size_t size = keys.size();

	for (std::size_t i = 0; i < size; i++)
	{
		m_resources.emplace(keys[i], std::make_unique<T>());
		m_resources.find(keys[i])->second->loadFromFile(paths[i]);
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
inline void ResourceLoader<sf::Shader>::LoadResources(const std::vector<std::string>& keys, const std::vector<std::string>& paths)
{
	if (keys.size() != paths.size())
		return;

	std::size_t size = keys.size();

	for (std::size_t i = 0; i < size; i++)
	{
		auto pos = paths[i].find(".");
		auto type = ShaderTypeFromFileExtension(paths[i].substr(pos + 1));
		if (type)
		{
			m_resources.emplace(keys[i], std::make_unique<sf::Shader>());
			m_resources.find(keys[i])->second->loadFromFile(paths[i], *type);
		}
	}
}

template<>
inline void ResourceLoader<sf::Music>::LoadResources(const std::vector<std::string>& keys, const std::vector<std::string>& paths)
{
	if (keys.size() != paths.size())
		return;

	std::size_t size = keys.size();

	for (std::size_t i = 0; i < size; i++)
	{
		auto pos = paths[i].find(".");
		auto type = ShaderTypeFromFileExtension(paths[i].substr(pos + 1));
		if (type)
		{
			m_resources.emplace(keys[i], std::make_unique<sf::Music>());
			m_resources.find(keys[i])->second->openFromFile(paths[i]);
		}
	}
}

template<typename T>
inline T* ResourceLoader<T>::GetResource(std::string name)
{
	std::map<std::string, std::unique_ptr<T>> resources;

	if (auto search = resources.find(name); search != resources.end())
		return search->second.get();

	return nullptr;
}