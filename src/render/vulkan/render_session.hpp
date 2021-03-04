#pragma once
#include "vkcommon.h"

#define MAIN_VER 0
#define MID_VER  0
#define END_VER  1

namespace cruelEngine {
    class RenderContext;
    class Window;

namespace VulkanContext {
    class LogicalDevice;
    class PhysicalDevice;
    class CommandBuffer;
    class Swapchain;
    class RenderPass;
    class FrameBuffer;
    class Instance;
    class RenderTask;
    class Queue;

    struct RenderProp {
            bool        validation = true;

            const std::vector<const char*>
                        validationLayers = {};
            const std::vector<const char*>
                        enabledInstanceExtensions = {};

            const std::vector<const char*>
                        enabledDeviceExtensions = {};

            bool        vsync = false;
            bool        overlay = false;
    };

    class RenderSession {
    public:
        RenderSession (Instance &instance, LogicalDevice &device, RenderProp &render_properties);

        RenderSession (const RenderSession &) = delete;

        RenderSession &operator=(const RenderSession &) = delete;

        ~RenderSession();

        // different types of update_swapchain
        void            update_swapchain() {}

        void            recreate_swapchain() {}

        RenderTask      *request_new_task();

        void            prepare();

        void            prepare_render_pass();

        void            draw();

        void            render_frame();

        LogicalDevice   &get_device() const {return device;}

        Swapchain       &get_swapchain() const {return *swapchain;}

        void            createSemaphores();

        void            destroySemaphores();

    private:
        const int                   MAX_FRAMES_IN_FLIGHT = 2;
        VkSemaphore                 imageAvailableSemaphore;
        VkSemaphore                 renderFinishedSemaphore;
        std::vector<VkSemaphore>    imageAvailableSemaphores;
        std::vector<VkSemaphore>    renderFinishedSemaphores;

        std::vector<VkFence>        inFlightFences;
        std::vector<VkFence>        imagesInFlight;
        size_t                      currentFrame = 0;

        Instance        &instance;

        LogicalDevice   &device;

        std::unique_ptr<Swapchain>  swapchain;

        std::unique_ptr<RenderPass> render_pass;

        RenderProp      render_properties;

        Queue           *graphic_queue;
        Queue           *present_queue;

        std::vector<std::unique_ptr<FrameBuffer>> frameBuffer;

        std::vector<std::reference_wrapper<CommandBuffer>> commandBuffers;

        std::vector<std::unique_ptr<RenderTask>> tasks;
    };

}
}