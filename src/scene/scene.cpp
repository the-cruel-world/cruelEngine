#include "scene.hpp"

#include "object.hpp"
#include "objects/triangle.hpp"

namespace cruelEngine {
namespace cruelScene {
Scene::Scene(cruelRender::RenderContext &render_context)
    : render_context{render_context} {}

Scene::~Scene() { sceneObjects.clear(); }

void Scene::addObject(std::string name, u32 session_idx, ObjectType type) {
  if (type == SCENE_OBJ_TYPE_TRIANGLE)
    sceneObjects.push_back(
        std::make_unique<Triangle>(render_context, session_idx, name));
}

void Scene::rmObject(std::string name) {}

void Scene::update() {}

void Scene::render() {}
} // namespace cruelScene
} // namespace cruelEngine