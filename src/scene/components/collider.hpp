#pragma once
#include "../component.hpp"

namespace cruelEngine::cruelScene
{
enum class ColliderShape
{
    COLLIDER_SHAPE_BOX = 0,
    COLLIDER_SHAPE_SPHERE,
    COLLIDER_SHAPE_MESH,
};

class Collider : public Component
{
public:
    Collider(const std::string &name, ColliderShape shapeType = ColliderShape::COLLIDER_SHAPE_BOX);

    const ColliderShape GetShapeType() const
    {
        return shapeType;
    }

private:
    ColliderShape shapeType = ColliderShape::COLLIDER_SHAPE_BOX;
};
} // namespace cruelEngine::cruelScene