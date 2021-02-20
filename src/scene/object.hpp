#pragma once
#include "../render/vulkan/pipeLine.hpp"
#include "../render/vulkan/shader.hpp"

#include "mesh.hpp"

namespace cruelEngine {
namespace cruelScene {
    
    struct Object {
    public:
        Object () {}
        virtual ~Object () {}

        void    drawCommand();

        void    loadAsset();

        void    unloadAsset();

    private:
        char        name[255];
        u32         lifetime;

        std::vector<Mesh> meshes;
        std::vector<Texture> textures;
        std::vector<Vertex> vertices;

        //PipeLine    m_pipeline;
        //Shader      m_shader;
    };
}
}