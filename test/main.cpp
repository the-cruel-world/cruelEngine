#include <algorithm>
#include <ctime>
#include <iostream>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unistd.h>
#include <vector>

// #include <GLFW/glfw3.h>
#include "../src/scene/scene_header.h"
#include <../src/window/window.hpp>

u32  cruelEngine::Window::count       = 0;
bool cruelEngine::Window::glfw_inited = false;

using namespace cruelEngine::cruelScene;

void print_tree(int level, std::shared_ptr<Node> root)
{
    for (size_t i = 0; i < level; i++)
    {
        std::cout << "  ";
    }
    if (level > 0)
        std::cout << "\\--";
    std::cout << "[Node " << root->GetId() << "]: " << root->GetName() << std::endl;

    if (root->GetChildren().size() == 0)
        return;
    else
    {
        for (auto &child : root->GetChildren())
        {
            print_tree(level + 1, child);
        }
    }
}

int main(int argc, char const *argv[])
{
    auto scene = std::make_unique<Scene>();

    auto nodes = scene->get_root_node();

    if (nodes == nullptr)
        return -1;

    nodes->AddChild(std::make_shared<Node>(1, "world"));

    nodes->GetChild("world")->AddChild(std::make_shared<Node>(1, "terrian"));

    nodes->GetChild("world")->AddChild(std::make_shared<Node>(1, "skybox"));

    nodes->AddChild(std::make_shared<Node>(1, "player1"));

    nodes->AddChild(std::make_shared<Node>(1, "player2"));

    nodes->GetChild("player2")->AddChild(std::make_shared<Node>(1, "clothes"));

    print_tree(0, nodes);

    return 0;
}