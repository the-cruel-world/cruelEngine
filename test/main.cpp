#include <algorithm>
#include <ctime>
#include <iostream>
#include <memory>
#include <typeindex>
#include <typeinfo>
#include <unistd.h>
#include <vector>

#include "../src/render/render_header.h"
#include "../src/scene/scene_header.h"
#include <../src/window/window.hpp>

u32  cruelEngine::Window::count       = 0;
bool cruelEngine::Window::glfw_inited = false;

using namespace cruelEngine::cruelScene;
using namespace cruelEngine::cruelRender;

using namespace std;

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