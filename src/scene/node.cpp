#include "node.hpp"

namespace cruelEngine::cruelScene
{
Node::Node(const size_t id, const std::string &name) : id{id}, name{name}
{}

void Node::SetParent(std::shared_ptr<Node> node)
{
    parent.reset();
    parent = std::move(node);
}

void Node::AddChild(std::shared_ptr<Node> node)
{
    children.push_back(std::move((node)));
}

} // namespace cruelEngine::cruelScene