#include "scene.hpp"
#include "object.hpp"

namespace cruelEngine::cruelScene {
Scene::Scene() {}

Scene::~Scene() { sceneObjects.clear(); }

void Scene::addObject(std::unique_ptr<Object> &obj) {
  sceneObjects.push_back(std::move(obj));
}

void Scene::rmObject(std::string name) {}

void Scene::update() {}

void Scene::render() {}
} // namespace cruelEngine::cruelScene