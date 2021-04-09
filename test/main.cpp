#include <algorithm>
#include <ctime>
#include <iostream>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unistd.h>
#include <vector>

#include "render/render_header.h"
#include "scene/scene_header.h"
#include "window/window.hpp"

u32  cruelEngine::Window::count       = 0;
bool cruelEngine::Window::glfw_inited = false;

using namespace cruelEngine::cruelScene;
using namespace cruelEngine::cruelRender;

using namespace std;

void print_tree(int level, Node *root)
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

class test
{
public:
    test(int a) : elm{a}
    {
        cout << "create" << endl;
    }

    test(const test &other) : elm{other.elm}
    {
        cout << "copy" << endl;
    }

    test(test &&other) : elm{other.elm}
    {
        cout << "move" << endl;
        other.elm = 0;
    }
    ~test()
    {
        cout << "Destroy" << endl;
    };

    int elm = 1000;
};

void sprite(test &&other)
{
    test val2 = move(other);
    cout << "sprite: " << val2.elm << endl;
}

int main(int argc, char const *argv[])
{
    {
        /**
         * Scene Tree structure test.
         */
        cout << "Scene Tree structure test." << endl;
        auto scene = std::make_unique<Scene>("Scene");

        std::vector<std::unique_ptr<Node>> nodes;

        nodes.push_back(std::make_unique<Node>(0, "world"));
        nodes.push_back(std::make_unique<Node>(1, "terrian"));
        nodes.push_back(std::make_unique<Node>(1, "skybox"));
        nodes.push_back(std::make_unique<Node>(2, "player1"));
        nodes.push_back(std::make_unique<Node>(2, "player2"));
        nodes.push_back(std::make_unique<Node>(3, "clothes"));

        scene->set_root_node(*nodes[0]);

        scene->add_nodes(std::move(nodes));

        print_tree(0, scene->get_root_node());
    }

    {
        /**
         * CommandBuffer and CommandPool test.
         */
        auto renderprop = std::make_unique<RenderProp>();
        cout << "CommandBuffer and CommandPool test." << endl;
        auto context = std::make_unique<RenderContext>(*renderprop);

        auto commandPool = std::make_unique<CommandPool>(
            context->get_device(),
            context->get_device().get_suitable_graphics_queue(0).get_family_index());

        for (size_t i = 0; i < 5; i++)
        {
            CommandBuffer &cmd = commandPool->RequestCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);
            cmd.Release();
            commandPool->test_list_commands();
        }
    }

    {
        // vector<test> mylists{};
        test val(21);
        // mylists.push_back(move(val));
        // test val2(std::move(val));
        // sprite(move(val));
        cout << "ori: " << val.elm << endl;
        // cout << "sprite: " << val2.elm << endl;
    }

    return 0;
}