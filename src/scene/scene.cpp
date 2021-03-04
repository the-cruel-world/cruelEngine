#include "scene.hpp"
#include "object.hpp"

namespace cruelEngine {
namespace cruelScene {
    Scene::Scene(cruelRender::RenderContext &render_context) :
        render_context {render_context}
    {

    }

    Scene::~Scene()
    {

    }

    void Scene::addObject(std::string name)
    {
        sceneObjects.push_back(std::make_unique<Object>(render_context, name));
    }

    void Scene::rmObject(std::string name)
    {

    }

    void Scene::update()
    {

    }

    void Scene::render()
    {

    }
}
}