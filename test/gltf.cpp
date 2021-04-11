#include "scene/scene_header.h"
#include "tiny_gltf.h"

#include <chrono>
#include <iostream>

using namespace tinygltf;
using namespace std;
using namespace cruelEngine;

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Usage: \n"
               "  <%s>   <gltf model path>\n",
               argv[0]);
        return -1;
    }

    Model       model;
    TinyGLTF    loader;
    std::string err;
    std::string warn;

    auto start_marker = std::chrono::high_resolution_clock::now();

    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, argv[1]);

    if (!warn.empty())
    {
        printf("Warn: %s\n", warn.c_str());
    }

    if (!err.empty())
    {
        printf("Err: %s\n", err.c_str());
    }

    if (!ret)
    {
        printf("Failed to parse glTF\n");
        return -1;
    }
    cout << "model loaded" << endl;

    float duration = std::chrono::duration<float, std::chrono::milliseconds::period>(
                         std::chrono::high_resolution_clock::now() - start_marker)
                         .count();

    cout << "Time usage: " << duration << " ms" << endl;

    cruelScene::Scene scene("Myscene");

    std::vector<std::unique_ptr<cruelScene::Node>> nodes{};

    cout << "Nodes: " << model.nodes.size() << endl;

    for (size_t node_index = 0; node_index < model.nodes.size(); ++node_index)
    {
        auto &gltf_node  = model.nodes[node_index];
        auto  scene_node = make_unique<cruelScene::Node>(node_index, gltf_node.name);

        cout << "Node " << node_index << ": " << gltf_node.name << endl;
        nodes.push_back(std::move(scene_node));
    }

    scene.set_root_node(*nodes.at(0));

    scene.add_nodes(std::move(nodes));

    cout << "scene created" << endl;
    return 0;
}
