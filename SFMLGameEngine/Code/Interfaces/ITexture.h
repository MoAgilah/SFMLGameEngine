#pragma once

#include "../Utilities/Point.h"
#include <string>
#include <memory>

class ITexture
{
public:
	virtual ~ITexture() = default;

    virtual bool LoadFromFile(const std::string& path) = 0;
    virtual Point GetTextureSize() const = 0;

    template<typename T>
    T* As() { return static_cast<T*>(GetNativeHandle()); }

protected:

    virtual void* GetNativeHandle() = 0;
};