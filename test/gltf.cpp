#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "tiny_gltf.h"

using namespace tinygltf;

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

    bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, argv[1]);
    // bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, argv[1]); // for binary glTF(.glb)

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

    return 0;
}
