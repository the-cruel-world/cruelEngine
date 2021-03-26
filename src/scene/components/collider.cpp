#include "collider.hpp"

namespace cruelEngine::cruelScene
{
Collider::Collider(const std::string &name, ColliderShape shapeType) :
    Component{name}, shapeType{shapeType}
{}
} // namespace cruelEngine::cruelScene