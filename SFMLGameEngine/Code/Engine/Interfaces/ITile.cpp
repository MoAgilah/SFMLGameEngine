#include "ITile.h"

ITile::ITile(std::shared_ptr<IDrawable> text, std::shared_ptr<IDrawable> slope, std::shared_ptr<IBoundingBox> aabb)
	: m_text(std::move(text)), m_slope(std::move(slope)), m_aabb(std::move(aabb))
{
}

void ITile::Render(IRenderer* renderer)
{

}

bool ITile::Intersects(IDynamicGameObject* obj)
{
    return m_aabb->Intersects(obj->GetVolume());
}

Point ITile::GetSeperationVector(IDynamicGameObject* obj)
{
	return m_aabb->GetSeparationVector(obj->GetVolume());
}