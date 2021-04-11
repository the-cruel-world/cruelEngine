#include "mesh.hpp"

namespace cruelEngine::cruelScene
{
Mesh::Mesh(const std::string &name) : Component(name)
{
}

void Mesh::UpdateBounds()
{}

const AABB &Mesh::GetBounds() const
{
    return bounds;
}

std::type_index Mesh::GetType()
{
    return typeid(Mesh);
}
void Mesh::AddPrimitive(Primitive &primitive)
{
    primitives.push_back(&primitive);
}

} // namespace cruelEngine::cruelScene