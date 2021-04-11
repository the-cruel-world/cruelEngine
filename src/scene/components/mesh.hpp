#pragma once
#include "scene/component.hpp"
#include "scene/node.hpp"
#include "scene/components/primitive.hpp"
#include "scene/components/aabb.hpp"

namespace cruelEngine::cruelScene
{
/**
 * \brief Mesh stores the geom data.
 */
class Mesh : public Component
{
public:
    Mesh(const std::string &name);

    ~Mesh() = default;

    void UpdateBounds();

    const AABB &GetBounds() const;

    std::type_index GetType() override;

    void AddPrimitive(Primitive &primitive);

private:
    AABB bounds;

    std::vector<Primitive *> primitives;

    std::vector<Node *> nodes;
};
} // namespace cruelEngine::cruelScene