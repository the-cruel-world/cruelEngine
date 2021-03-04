#pragma once

#include "../common.h"
#include "../types.h"
#include "mesh.hpp"

namespace cruelEngine {
namespace VulkanContext {
    class RenderTask;
    class VulkanContext;
    class ShaderModule;
}

namespace cruelScene {

    struct Mesh;
    struct Texture;
    struct Vertex;
    
    class Object {
    public:
        Object (VulkanContext::VulkanContext &render_context, std::string name);

        virtual ~Object();

        void    prepare();

        void    drawCommand();

        virtual void    loadAsset() {};

        virtual void    unloadAsset() {};

        const std::string &get_name() const {return name;}

        bool            is_alive() const;

        void            show_name() const {std::cout << "Object : " << name << std::endl;}

        VulkanContext::RenderTask   *get_task() {return task;}

    protected:
        std::string     name;
        // std::chrono::high_resolution_clock         lifetime;

        std::vector<Mesh> meshes;
        std::vector<Texture> textures;
        std::vector<Vertex> vertices;

        std::vector<VulkanContext::ShaderModule> shaders;

        VulkanContext::VulkanContext &render_context;
        VulkanContext::RenderTask *task;
    };
}
}