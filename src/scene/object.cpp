#include "object.hpp"
#include "mesh.hpp"

namespace cruelEngine::cruelScene {
Object::Object(std::string name) : name{name} { show_name(); }

Object::~Object() { mesh.clear(); }

void Object::prepare() {}

} // namespace cruelEngine::cruelScene