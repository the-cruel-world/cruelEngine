#include "node.hpp"

namespace cruelEngine::cruelScene
{
Node::Node(const size_t id, const std::string &name) : id{id}, name{name}
{}

void Node::SetId(size_t new_id)
{
    id = new_id;
}

size_t Node::GetId() const
{
    return id;
}

const std::string &Node::GetName() const
{
    return name;
}

void Node::SetParent(Node &node)
{
    parent = &node;

    /** \todo After set parent, new transform needed. */
}

const Node *Node::GetParent() const
{
    return parent;
}

void Node::AddChild(Node &node)
{
    children.push_back(&node);
}

const Node *Node::GetChild(size_t id)
{
    for (auto &child : children)
    {
        if (child->GetId() == id)
            return child;
    }
    return nullptr;
}

const Node *Node::GetChild(std::string name)
{
    for (auto &child : children)
    {
        if (child->GetName() == name)
            return child;
    }
    return nullptr;
}

const std::vector<Node *> &Node::GetChildren() const
{
    return children;
}
Component &Node::GetComponent(const std::type_index index)
{
    return *components.at(index);
}
template <class T>
T &Node::GetComponent()
{
    return dynamic_cast<T &>(GetComponent(typeid(T)));
}
void Node::SetComponent(Component &new_component)
{
    auto it = components.find(new_component.GetType());
    if (it != components.end())
    {
        it->second = &new_component;
    }
    else
    {
        components.insert(std::make_pair(new_component.GetType(), &new_component));
    }
}

} // namespace cruelEngine::cruelScene