#pragma once
#include "vkcommon.h"
#include "../render_context.hpp"

namespace cruelEngine {
    class RenderContext;
    class Window;

namespace VulkanContext {

    class LogicalDevice;
    class PhysicalDevice;
    class Swapchain;
    class Instance;
    class RenderTask;
    class RenderSession;

    class VulkanContext : public RenderContext
    {
    public:
        VulkanContext (const Window& _window);

        ~VulkanContext();

        void    prepareRender();

        void    draw();

        void    render_frame();

        Instance        &get_instance() const {return *instance;}

        PhysicalDevice  &get_gpu() const {return *physicalDevice;}

        LogicalDevice   &get_device() const {return *device;}

        RenderSession   &get_session() const {return *session;}

    protected:
        VkApplicationInfo       appInfo = {
            .sType=VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "Triangle",
            .pEngineName = "Real Engine",
            .apiVersion = VK_API_VERSION_1_0};

        std::unique_ptr<Instance>           instance;

        std::unique_ptr<PhysicalDevice>     physicalDevice;

        std::unique_ptr<LogicalDevice>      device;

        std::unique_ptr<RenderSession>      session;
    };
}
}