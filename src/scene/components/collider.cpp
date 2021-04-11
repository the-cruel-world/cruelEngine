#include "collider.hpp"

namespace cruelEngine::cruelScene
{
Collider::Collider(const std::string &name, ColliderShape shapeType) :
    Component{name}, shapeType{shapeType}
{}
std::type_index Collider::GetType()
{
    return typeid(Collider);
}
} // namespace cruelEngine::cruelScene