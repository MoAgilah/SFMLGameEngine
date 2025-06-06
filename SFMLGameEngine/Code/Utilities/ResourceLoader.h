#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <memory>
#include <iostream>
#include <optional>
#include <filesystem>

namespace fs = std::filesystem;

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
		auto resource = std::make_unique<T>();

		if (!resource->loadFromFile(entry.path().string()))
		{
			// Handle the error (log it, throw, continue, etc.)
			std::cerr << "Failed to load resource: " << entry.path() << "\n";
			continue;
		}

		m_resources.emplace(filename, std::move(resource));
	}
}

template<>
inline void ResourceLoader<sf::Font>::LoadResources(fs::path path)
{
	if (!fs::exists(path))
		fs::create_directory(path);

	for (const auto& entry : fs::directory_iterator(path))
	{
		auto filename = entry.path().filename().replace_extension().string();
		auto font = std::make_unique<sf::Font>();

		if (!font->openFromFile(entry.path().string()))
		{
			std::cerr << "Failed to load font: " << entry.path() << "\n";
			continue; // or throw, or handle the error
		}

		m_resources.emplace(filename, std::move(font));
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
			auto shader = std::make_unique<sf::Shader>();

			if (!shader->loadFromFile(entry.path().string(), *type))
			{
				std::cerr << "Failed to load shader: " << entry.path() << "\n";
				continue; // or throw, depending on your error policy
			}

			m_resources.emplace(filename, std::move(shader));
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
		auto music = std::make_unique<sf::Music>();

		if (!music->openFromFile(entry.path().string()))
		{
			std::cerr << "Failed to load music: " << entry.path() << "\n";
			continue; // or throw an exception, depending on your needs
		}

		m_resources.emplace(filename, std::move(music));
	}

}

template<typename T>
inline T* ResourceLoader<T>::GetResource(std::string name)
{
	if (auto search = m_resources.find(name); search != m_resources.end())
		return search->second.get();

	return nullptr;
}