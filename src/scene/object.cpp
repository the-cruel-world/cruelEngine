#include "object.hpp"

#include "components/camera.hpp"

namespace cruelEngine::cruelScene
{
Object::Object(std::string name, Camera &camera) : name{name}, camera{camera}
{}

Object::~Object()
{
    // mesh.clear();
}

// void Object::prepare() {}

} // namespace cruelEngine::cruelScene