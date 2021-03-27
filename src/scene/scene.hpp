#pragma once
#include "../common.h"
#include "component.hpp"
#include "components/camera.hpp"
#include "node.hpp"
#include "object.hpp"

namespace cruelEngine::cruelScene
{
class Object;

class Scene
{
public:
    Scene();
    virtual ~Scene();

    void addObject(std::shared_ptr<Object> obj);

    void rmObject(std::string name);

    const std::vector<std::shared_ptr<Object>> &get_objs() const
    {
        return sceneObjects;
    }

    void add_node(std::shared_ptr<Node> node);

    const std::vector<std::shared_ptr<Node>> &get_nodes() const;

    void set_root_node(std::shared_ptr<Node> node)
    {
        root = std::move(node);
    }

    std::shared_ptr<Node> get_root_node()
    {
        return root;
    }

    void add_component(std::shared_ptr<Component> component);

    void prepare_camera();

    Camera &get_camera();

    void update();

    void render();

protected:
    std::vector<std::shared_ptr<Object>> sceneObjects{};

    std::unique_ptr<Camera> camera;

    /**
     * All nodes in this scene.
     */
    std::vector<std::shared_ptr<Node>> nodes;

    /**
     * pointer to the root node.
     */
    std::shared_ptr<Node> root = nullptr;

    Node *find_node(const std::string &name);

    /**
     * Components
     */
    std::unordered_map<std::type_index, std::vector<std::shared_ptr<Component>>> components;
};
} // namespace cruelEngine::cruelScene