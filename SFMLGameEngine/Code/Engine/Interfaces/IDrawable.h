#pragma once

#pragma once

#include "IRenderable.h"
#include "IUpdatable.h"
#include "ITransforms.h"

class IDrawableNoUpdate : public IRenderable, public ITransforms
{
public:
	virtual ~IDrawableNoUpdate() = default;
};

class IDrawable : public IRenderable, public IUpdatable, public ITransforms
{
public:
	virtual ~IDrawable() = default;
};
