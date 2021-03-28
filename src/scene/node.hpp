
#pragma once
#include "../common.h"
// #include "component.hpp"
#include "components/transform.hpp"
namespace cruelEngine::cruelScene
{
class Component;

class Node
{
public:
    Node(const size_t id, const std::string &name);

    virtual ~Node() = default;

    void update();

    void SetParent(std::shared_ptr<Node> node);

    const std::shared_ptr<Node> &GetParent() const;

    void SetId(size_t new_id);

    size_t GetId() const;

    const std::string &GetName() const;

    void AddChild(std::shared_ptr<Node> node);

    const std::shared_ptr<Node> &GetChild(size_t id);

    const std::shared_ptr<Node> &GetChild(std::string name);

    const std::vector<std::shared_ptr<Node>> &GetChildren() const;

protected:
    size_t id;

    std::string name;

    std::shared_ptr<Node> parent;

    uint32_t index;

    Transform transform;

    std::vector<std::shared_ptr<Node>> children;

    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
};

} // namespace cruelEngine::cruelScene