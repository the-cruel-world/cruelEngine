#pragma once
#include "scene/component.hpp"
#include "scene/components/camera.hpp"
#include "scene/node.hpp"
#include "scene/object.hpp"
#include "sgcommon.h"

namespace cruelEngine::cruelScene
{
class Object;

class Scene
{
public:
    Scene(const std::string &name);
    virtual ~Scene();

    const std::string &get_name() const;

    void set_name(const std::string &new_name);

    void add_node(std::unique_ptr<Node> &&node);

    void add_nodes(std::vector<std::unique_ptr<Node>> &&new_nodes);

    /**
     * The input node must be in the nodes list of the scene.
     */
    void set_root_node(Node &node)
    {
        root = &node;
        root->SetParent(*root); // root's parent points to itself
    }

    Node *get_root_node()
    {
        return root;
    }

    void add_component(std::unique_ptr<Component> component);

    void prepare_camera();

    Camera &get_camera();

    void update();

    void render();

protected:
    std::string name;

    std::vector<std::shared_ptr<Object>> sceneObjects{};

    std::unique_ptr<Camera> camera;

    /**
     * pointer to the root node.
     */
    Node *root = nullptr;

    Node *find_node(const std::string &name);

    /**
     * All nodes in this scene.
     */
    std::vector<std::unique_ptr<Node>> nodes;

    /**
     * Components
     */
    std::unordered_map<std::type_index, std::vector<std::unique_ptr<Component>>> components;
};
} // namespace cruelEngine::cruelScene