#pragma once

#include <memory>
#include "../Code/Drawables/Sprite.h"

template<typename T>
class GraphicsObject
{
public:
	GraphicsObject() = default;
	GraphicsObject(const std::string& name) = delete;
	~GraphicsObject() = default;

	template<typename T>
	inline T* GraphicsObject<T>::GetObject()
	{
		return m_object.get();
	}

private:

	std::unique_ptr<T> m_object;
};

template<>
inline GraphicsObject<Sprite>::GraphicsObject(const std::string& name)
{
}

template<>
inline GraphicsObject<AnimatedSprite>::GraphicsObject(const std::string& name)
{
}