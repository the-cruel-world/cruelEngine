#pragma once
#include "scene/component.hpp"
#include "scene/components/aabb.hpp"

namespace cruelEngine::cruelScene
{
struct VertexAttribute
{
    u32 stride = 0;

    u32 offset = 0;
};

/**
 * \brief Mesh2 stores the geom data.
 */
class Mesh2 : Component
{
    Mesh2(const std::string &name);

    ~Mesh2() = default;

    void UpdateBounds(){};

    const AABB &GetBounds() const
    {
        return bounds;
    }

private:
    AABB bounds;

    std::vector<VertexAttribute> vertexAttributes{};
};
} // namespace cruelEngine::cruelScene