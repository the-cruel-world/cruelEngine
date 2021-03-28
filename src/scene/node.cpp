#include "node.hpp"

namespace cruelEngine::cruelScene
{
Node::Node(const size_t id, const std::string &name) : id{id}, name{name}
{}

const std::shared_ptr<Node> &Node::GetParent() const
{
    return parent;
}

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

void Node::SetParent(std::shared_ptr<Node> node)
{
    parent.reset();
    parent = std::move(node);
}

void Node::AddChild(std::shared_ptr<Node> node)
{
    children.push_back(std::move((node)));
}

const std::shared_ptr<Node> &Node::GetChild(size_t id)
{
    for (auto &child : children)
    {
        if (child->GetId() == id)
            return child;
    }
    return std::forward<std::shared_ptr<Node>>(nullptr);
}

const std::shared_ptr<Node> &Node::GetChild(std::string name)
{
    for (auto &child : children)
    {
        if (child->GetName() == name)
            return child;
    }
    return std::forward<std::shared_ptr<Node>>(nullptr);
}

const std::vector<std::shared_ptr<Node>> &Node::GetChildren() const
{
    return children;
}

} // namespace cruelEngine::cruelScene