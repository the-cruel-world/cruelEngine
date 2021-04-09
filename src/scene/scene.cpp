#include "scene.hpp"

#include "object.hpp"

namespace cruelEngine::cruelScene
{
Scene::Scene(const std::string &name) : name{name}
{
    prepare_camera();
}

Scene::~Scene()
{
    sceneObjects.clear();
}

const std::string &Scene::get_name() const
{
    return name;
}

void Scene::set_name(const std::string &new_name)
{
    name = new_name;
}

void Scene::add_node(std::unique_ptr<Node> &&node)
{
    nodes.emplace_back(std::move(node));
}

void Scene::add_nodes(std::vector<std::unique_ptr<Node>> &&new_nodes)
{
    assert(nodes.empty() && "Scene already in use. can't reset nodes, create a new scene instead.");

    nodes = std::move(new_nodes);
}

void Scene::add_component(std::unique_ptr<Component> component)
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