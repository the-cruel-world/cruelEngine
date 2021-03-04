#pragma once
#include "vkcommon.h"

namespace cruelEngine {
    class Window;

namespace cruelRender {

    class LogicalDevice;
    class PhysicalDevice;
    class Swapchain;
    class Instance;
    class RenderTask;
    class RenderSession;
    struct RenderProp;

    class RenderContext
    {
    public:
        RenderContext ();

        ~RenderContext();

        void    prepareRender();

        void    draw();

        void    render_frame();

        Instance        &get_instance() const {return *instance;}

        PhysicalDevice  &get_gpu() const {return *physicalDevice;}

        LogicalDevice   &get_device() const {return *device;}

        RenderSession   &get_session(u32 index) const ;

        void            add_session(std::string session_name, RenderProp &properties);

        bool            is_context_alive();


    protected:
        VkApplicationInfo       appInfo = {
            .sType=VK_STRUCTURE_TYPE_APPLICATION_INFO,
            .pApplicationName = "Triangle",
            .pEngineName = "Real Engine",
            .apiVersion = VK_API_VERSION_1_0};

        /*! \brief The Vulkan instance. 
            In general, one application should have only one vulkan instance.
            Because multi vulkan instance may crush your applications. */
        std::unique_ptr<Instance>           instance;

        /*! \brief The physical device (aka gpu). */
        std::unique_ptr<PhysicalDevice>     physicalDevice;

        /*! \brief The logical device of this render context. */
        std::unique_ptr<LogicalDevice>      device;

        /*! \brief The render session container. 
            Instead of let the render context to control everything, I use render sessions for 
            muiti window(surface) management. There should be more sessions. */
        std::vector<std::unique_ptr<RenderSession>>     sessions;
    };
}
}