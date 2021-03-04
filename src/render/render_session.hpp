#pragma once
#include "vkcommon.h"

#define MAIN_VER 0
#define MID_VER  0
#define END_VER  1

namespace cruelEngine {
    class RenderContext;
    class Window;

namespace cruelRender {
    class LogicalDevice;
    class PhysicalDevice;
    class CommandBuffer;
    class Swapchain;
    class RenderPass;
    class FrameBuffer;
    class Instance;
    class RenderTask;
    class Queue;

    //Todo 
    // 1. The rendersession should support headless render.
    // 2. Session should support multi swapchains/renderpass.
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

        Window          &get_window() const {return *window;}

        VkSurfaceKHR    &get_surface() {return surface;}

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

        //! \brief The reference points to the instance of the render context
        Instance        &instance;
        
        //! \brief The reference points to the logical device of the render context.
        LogicalDevice   &device;

        //! \brief The unique window of a render session, it's null if the session is a headless session.
        std::unique_ptr<Window>     window;

        //! \brief The surface of a session. Every session will have one unless it's a headless render session.
        VkSurfaceKHR                surface; 

        //! \brief The swapchain of this session. Every session should have at least one swapchain.
        std::unique_ptr<Swapchain>  swapchain;

        //! \brief The renderpass of this session. Every session should have at least one render pass. 
        std::unique_ptr<RenderPass> render_pass;

        //! \brief The redner properties of a render session.
        RenderProp      render_properties;

        Queue           *graphic_queue;
        Queue           *present_queue;

        std::vector<std::unique_ptr<FrameBuffer>> frameBuffer;

        std::vector<std::reference_wrapper<CommandBuffer>> commandBuffers;

        std::vector<std::unique_ptr<RenderTask>> tasks;
    };

}
}