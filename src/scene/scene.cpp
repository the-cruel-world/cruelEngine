#include "scene.hpp"
#include "object.hpp"

namespace cruelEngine::cruelScene {
Scene::Scene() { prepare_camera(); }

Scene::~Scene() { sceneObjects.clear(); }

void Scene::addObject(std::shared_ptr<Object> obj) {
  sceneObjects.push_back(std::move(obj));
  // obj.reset();
}

void Scene::rmObject(std::string name) {}

void Scene::update() {}

void Scene::render() {}

void Scene::prepare_camera() { camera = std::make_unique<Camera>("camera"); }

Camera &Scene::get_camera() { return *camera; }
} // namespace cruelEngine::cruelScene