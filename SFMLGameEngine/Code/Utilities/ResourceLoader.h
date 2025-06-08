#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <optional>
#include <filesystem>
#include <algorithm>

namespace fs = std::filesystem;

template<typename T>
class ResourceLoader
{
public:
	explicit ResourceLoader(const std::string& path) { LoadResources(path); }
	ResourceLoader() = default;
	virtual ~ResourceLoader() = default;

	T* GetResource(const std::string& name);

	void LoadResources(const fs::path& path);

private:
	std::unordered_map<std::string, std::unique_ptr<T>> m_resources;

	bool IsValidDirectory(const fs::path& path) const;
	std::string GetCleanName(const fs::path& path) const;
};

// General Resource Loader
template<typename T>
void ResourceLoader<T>::LoadResources(const fs::path& path)
{
	if (!IsValidDirectory(path)) return;

	for (const auto& entry : fs::directory_iterator(path))
	{
		auto resource = std::make_unique<T>();
		if (!resource->loadFromFile(entry.path().string()))
		{
			std::cerr << "Failed to load resource: " << entry.path() << "\n";
			continue;
		}
		m_resources.emplace(GetCleanName(entry.path()), std::move(resource));
	}
}

// Specialisation for sf::Font
template<>
inline void ResourceLoader<sf::Font>::LoadResources(const fs::path& path)
{
	if (!fs::exists(path) || !fs::is_directory(path)) return;

	for (const auto& entry : fs::directory_iterator(path))
	{
		auto font = std::make_unique<sf::Font>();
		if (!font->openFromFile(entry.path().string()))
		{
			std::cerr << "Failed to load font: " << entry.path() << "\n";
			continue;
		}
		m_resources.emplace(entry.path().filename().replace_extension().string(), std::move(font));
	}
}

// Specialisation for sf::Music
template<>
inline void ResourceLoader<sf::Music>::LoadResources(const fs::path& path)
{
	if (!fs::exists(path) || !fs::is_directory(path)) return;

	for (const auto& entry : fs::directory_iterator(path))
	{
		auto music = std::make_unique<sf::Music>();
		if (!music->openFromFile(entry.path().string()))
		{
			std::cerr << "Failed to load music: " << entry.path() << "\n";
			continue;
		}
		m_resources.emplace(entry.path().filename().replace_extension().string(), std::move(music));
	}
}

// Specialisation for sf::Shader
template<>
inline void ResourceLoader<sf::Shader>::LoadResources(const fs::path& path)
{
	auto ShaderTypeFromExtension = [](std::string ext) -> std::optional<sf::Shader::Type>
		{
		std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
		if (ext == "vert")
			return sf::Shader::Type::Vertex;
		if (ext == "frag")
			return sf::Shader::Type::Fragment;
		if (ext == "geom")
			return sf::Shader::Type::Geometry;

		return std::nullopt;
		};

	if (!fs::exists(path) || !fs::is_directory(path))
		return;

	for (const auto& entry : fs::directory_iterator(path))
	{
		auto type = ShaderTypeFromExtension(entry.path().extension().string().substr(1));
		if (!type) continue;

		auto shader = std::make_unique<sf::Shader>();
		if (!shader->loadFromFile(entry.path().string(), *type))
		{
			std::cerr << "Failed to load shader: " << entry.path() << "\n";
			continue;
		}
		m_resources.emplace(entry.path().filename().replace_extension().string(), std::move(shader));
	}
}

template<typename T>
T* ResourceLoader<T>::GetResource(const std::string& name)
{
	auto it = m_resources.find(name);
	return (it != m_resources.end()) ? it->second.get() : nullptr;
}

template<typename T>
bool ResourceLoader<T>::IsValidDirectory(const fs::path& path) const
{
	return fs::exists(path) && fs::is_directory(path);
}

template<typename T>
std::string ResourceLoader<T>::GetCleanName(const fs::path& path) const
{
	return path.filename().replace_extension().string();
}
