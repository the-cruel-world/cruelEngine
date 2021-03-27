#include "scene.hpp"

#include "object.hpp"

namespace cruelEngine::cruelScene
{
Scene::Scene()
{
    set_root_node(std::make_shared<Node>(0, "Root"));
    prepare_camera();
}

Scene::~Scene()
{
    sceneObjects.clear();
}

void Scene::addObject(std::shared_ptr<Object> obj)
{
    sceneObjects.push_back(std::move(obj));
    // obj.reset();
}

void Scene::rmObject(std::string name)
{}

void Scene::add_node(std::shared_ptr<Node> node)
{
    nodes.push_back(std::move(node));
}

const std::vector<std::shared_ptr<Node>> &Scene::get_nodes() const
{
    return nodes;
}

void Scene::add_component(std::shared_ptr<Component> component)
{
    components[typeid(component)].push_back(std::move(component));
}

void Scene::update()
{}

void Scene::render()
{}

void Scene::prepare_camera()
{
    camera = std::make_unique<Camera>("camera");
}

Camera &Scene::get_camera()
{
    return *camera;
}
} // namespace cruelEngine::cruelScene