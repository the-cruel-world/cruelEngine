#pragma once
#include "../common.h"
#include "../render/vulkan/vkheader.h"

namespace cruelEngine {
    class RenderContext;
namespace VulkanContext {
    class VulkanContext;
}

namespace cruelScene {
    class Object;

    class Scene {
    public:
        Scene (VulkanContext::VulkanContext &render_context);
        virtual ~Scene ();

        void addObject(std::string name);

        void rmObject(std::string name);

        const std::vector<std::unique_ptr<Object>> &get_obj() const {return sceneObjects;}

        void update();

        void render();

    protected:
        
        std::vector<std::unique_ptr<Object>> sceneObjects {};

        VulkanContext::VulkanContext   &render_context;
    };
}
}