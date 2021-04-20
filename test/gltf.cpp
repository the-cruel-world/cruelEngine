#include "scene/scene_header.h"
#include "tiny_gltf.h"

#include <chrono>
#include <iostream>
#include <queue>

using namespace tinygltf;
using namespace std;
using namespace cruelEngine;

inline std::vector<uint8_t> get_attribute_data(tinygltf::Model model, uint32_t accessorId)
{
    auto &accessor   = model.accessors.at(accessorId);
    auto &bufferView = model.bufferViews.at(accessor.bufferView);
    auto &buffer     = model.buffers.at(bufferView.buffer);

    size_t stride    = accessor.ByteStride(bufferView);
    size_t startByte = accessor.byteOffset + bufferView.byteOffset;
    size_t endByte   = startByte + accessor.count * stride;

    return {buffer.data.begin() + startByte, buffer.data.begin() + endByte};
};

// Function to load model from gltf file.
bool load_model(tinygltf::Model &model, const char *filename) {
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool res = loader.LoadASCIIFromFile(&model, &err, &warn, filename);
    if (!warn.empty()) {
        std::cout << "WARN: " << warn << std::endl;
    }

    if (!err.empty()) {
        std::cout << "ERR: " << err << std::endl;
    }

    if (!res)
        std::cout << "Failed to load glTF: " << filename << std::endl;
    else
        std::cout << "Loaded glTF: " << filename << std::endl;

    return res;
}

bool load_mesh(tinygltf::Model &model,
               cruelScene::Scene &scene)
{
    bool res = true;
    // load mesh;
    cout << "Loading Mesh to scene." << endl;
    for (auto &gltf_mesh : model.meshes)
    {
        cout << "\tMesh name: \"" << gltf_mesh.name + "\"" << endl;
        auto scene_mesh = std::make_unique<cruelScene::Mesh>(gltf_mesh.name);
        for (auto &gltf_primitive : gltf_mesh.primitives)
        {
            int32_t  bufferOffset, bufferSize, vertexCount;

            auto scene_primitive = std::make_unique<cruelScene::Primitive>();


            for (auto attribute : gltf_primitive.attributes)
            {
                const tinygltf::Accessor &posAccessor = model.accessors[attribute.second];
                const tinygltf::BufferView &posView = model.bufferViews[posAccessor.bufferView];

                bufferOffset = posAccessor.byteOffset + posView.byteOffset;
                vertexCount = static_cast<uint32_t>(posAccessor.count);

                std::string attr_name = attribute.first;
                cout << "\t\tAttribute name: \"" << attr_name + "\"\t: " << attribute.second << "\t offset: " << bufferOffset << "\tlength: " << vertexCount << "\t stride: " << posView.byteStride << endl;
            }

            auto attribute = gltf_primitive.attributes.find("POSITION");

            assert(attribute != gltf_primitive.attributes.end() && "Position attribute is necessary.");
            const tinygltf::Accessor &posAccessor = model.accessors[attribute->second];

            const tinygltf::BufferView &posView = model.bufferViews[posAccessor.bufferView];

            bufferOffset = posAccessor.byteOffset + posView.byteOffset;
            vertexCount = static_cast<uint32_t>(posAccessor.count);
            bufferSize = static_cast<uint32_t>(posAccessor.count * posView.byteStride);

            // fill data;
            char *data = scene_primitive->GetDataMutant();
            const char *src = reinterpret_cast<const char *>(&(model.buffers[posView.buffer].data[posAccessor.byteOffset + posView.byteOffset]));
            size_t dataSize = scene_primitive->GetDataSize();
            if (dataSize < bufferSize)
            {
                free(data);
                data = (char*)malloc(sizeof(char) * bufferSize);
                dataSize = bufferSize;
                scene_primitive->SetDataSize(bufferSize);
            }

            for (size_t i=0;i<vertexCount;i++)
            {
                memcpy(data, src, posView.byteStride);
                data += static_cast<uint32_t>(posView.byteStride);
                src += static_cast<uint32_t>(posView.byteStride);
            }

            scene_primitive->SetVertexLength(vertexCount);

            cruelScene::VertexAttribute position {cruelScene::VERTEX_FORMAT_R32G32B32_FLOAT, static_cast<uint32_t>(bufferOffset), static_cast<uint32_t>(posView.byteStride)};

            scene_primitive->SetVertexAttributes({position});       // Set the point attributes
            glm::vec3 min{posAccessor.minValues[0], posAccessor.minValues[1], posAccessor.minValues[2]};
            glm::vec3 max{posAccessor.maxValues[0], posAccessor.maxValues[1], posAccessor.maxValues[2]};
            scene_primitive->UpdateBounds(min, max);

            scene_mesh->AddPrimitive(*scene_primitive);
            scene.add_component(std::move(scene_primitive));
        }
        scene.add_component(std::move(scene_mesh));
    }
    return res;
}

// Function to build nodes from gltf file.
bool load_nodes(tinygltf::Model &model,
                cruelScene::Scene &scene)
{
    bool res = true;
    std::vector<std::unique_ptr<cruelScene::Node>> nodes{};
    auto meshes = scene.GetComponents<cruelScene::Mesh>();

    cout << "num of mesh: " << meshes.size() << endl;

    // create all entries for nodes in gltf scene.
    for (size_t node_index = 0; node_index < model.nodes.size(); ++node_index)
    {
        auto &gltf_node  = model.nodes[node_index];
        auto  scene_node = make_unique<cruelScene::Node>(node_index, gltf_node.name);

        if (gltf_node.mesh >= 0){
            // \todo
            // push new mesh
            scene_node->SetComponent(*(meshes.at(gltf_node.mesh)));
        }

        if (gltf_node.camera >= 0){
            // \todo
            // set camera
        }

        nodes.push_back(std::move(scene_node));
    }

    // Resolve the relationship for nodes.
    std::queue<std::pair<cruelScene::Node &, int>> traverse_nodes;
    const tinygltf::Scene & gltf_scene = model.scenes[0];

    auto root_node = std::make_unique<cruelScene::Node>(0, gltf_scene.name);
    for (size_t node_index = 0; node_index < gltf_scene.nodes.size(); ++node_index)
    {
        traverse_nodes.push(std::make_pair(std::ref(*root_node), node_index));
    }
    while (!traverse_nodes.empty())
    {
        auto node_it = traverse_nodes.front();
        traverse_nodes.pop();

        auto &traverse_root_node = node_it.first;
        auto &current_node       = nodes.at(node_it.second);

        current_node->SetParent(traverse_root_node);
        traverse_root_node.AddChild(*current_node);

        for (auto index : model.nodes[node_it.second].children)
        {
            traverse_nodes.push(std::make_pair(std::ref(*current_node), index));
        }
    }
    nodes.push_back(std::move(root_node));
    scene.set_root_node(*nodes.back());
    scene.add_nodes(std::move(nodes));

    return res;
}

// Output the structure of the scene nodes.
void print_tree(int level, cruelScene::Node *root)
{
    for (size_t i = 0; i < level; i++)
    {
        std::cout << "  ";
    }
    if (level > 0)
        std::cout << "\\--";
    std::cout << "[Node " << root->GetId() << "]: " << root->GetName() ;

    if (root->HasComponent<cruelScene::Mesh>())
    {
        auto &mesh = root->GetComponent<cruelScene::Mesh>();
        std::cout << "\t mesh: " << mesh.GetName() ;
    }
    std::cout << std::endl;

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

inline std::vector<uint8_t> get_attribute_data(const tinygltf::Model *model, uint32_t accessorId)
{
    auto &accessor   = model->accessors.at(accessorId);
    auto &bufferView = model->bufferViews.at(accessor.bufferView);
    auto &buffer     = model->buffers.at(bufferView.buffer);

    size_t stride    = accessor.ByteStride(bufferView);
    size_t startByte = accessor.byteOffset + bufferView.byteOffset;
    size_t endByte   = startByte + accessor.count * stride;

    return {buffer.data.begin() + startByte, buffer.data.begin() + endByte};
};

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Usage: \n"
               "  <%s>   <gltf model path>\n",
               argv[0]);
        return -1;
    }

    auto start_marker = std::chrono::high_resolution_clock::now();

    Model       model;

    // load the model and handle the errors.
    {
        bool ret = load_model(model, argv[1]);
        if (!ret)
        {
            printf("Failed to parse glTF\n");
            return -1;
        }
    }

    float duration = std::chrono::duration<float, std::chrono::milliseconds::period>(
                         std::chrono::high_resolution_clock::now() - start_marker)
                         .count();
    cout << "Time usage: " << duration << " ms" << endl;

    cruelScene::Scene scene("Myscene");
    cout << "scene created" << endl;

    load_mesh(model, scene);

    auto primitives = scene.GetComponents<cruelScene::Primitive>();

    for (auto primitive : primitives)
    {
        cout << "buffer len: " << primitive->GetDataSize() << "\t data len: " << primitive->GetVertexLength() << endl;
    }
    load_nodes(model, scene);

    print_tree(0, scene.get_root_node());

    return 0;
}