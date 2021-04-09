
#pragma once
#include "sgcommon.h"
// #include "component.hpp"
#include "scene/components/transform.hpp"
namespace cruelEngine::cruelScene
{
class Component;

class Node
{
public:
    Node(const size_t id, const std::string &name);

    virtual ~Node() = default;

    void update();

    void SetParent(Node &node);

    const Node *GetParent() const;

    void SetId(size_t new_id);

    size_t GetId() const;

    const std::string &GetName() const;

    void AddChild(Node &node);

    const Node *GetChild(size_t id);

    const Node *GetChild(std::string name);

    const std::vector<Node *> &GetChildren() const;

protected:
    size_t id;

    std::string name;

    uint32_t index;

    Node *parent{nullptr};

    std::vector<Node *> children;

    Transform transform;

    std::unordered_map<std::type_index, std::shared_ptr<Component>> components;
};

} // namespace cruelEngine::cruelScene