#pragma once
#include "vkcommon.h"

namespace cruelEngine
{
class Window;

namespace cruelRender
{
class LogicalDevice;
class PhysicalDevice;
class Swapchain;
class Instance;
class RenderTask;
class RenderSession;
struct RenderProp;
struct SessionProp;

struct RenderProp
{
    VkApplicationInfo appInfo = {.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                 .pApplicationName = "Cruel Engine",
                                 .pEngineName      = "Cruel Engine",
                                 .apiVersion       = USED_VULKAN_API_VERSION};

    bool validation = true;

    std::vector<const char *> validationLayers = {"VK_LAYER_KHRONOS_validation",
                                                  "VK_LAYER_LUNARG_monitor"};

    std::vector<const char *> enabledInstanceExtensions = {};

    std::vector<const char *> enabledDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    VkPhysicalDeviceFeatures features = {};

    VkQueueFlags flags = {VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_COMPUTE_BIT};

    bool vsync   = false;
    bool overlay = false;
};

class RenderContext
{
public:
    RenderContext(RenderProp properties);

    ~RenderContext();

    void prepareRender();

    void draw();

    void render_frame();

    Instance &get_instance() const
    {
        return *instance;
    }

    PhysicalDevice &get_gpu() const
    {
        return *physicalDevice;
    }

    LogicalDevice &get_device() const
    {
        return *device;
    }

    RenderSession &get_session(u32 index) const;

    void add_session(std::string session_name, SessionProp &properties);

    bool is_context_alive();

protected:
    RenderProp properties;

    /*! \brief The Vulkan instance.
      In general, one application should have only one vulkan instance.
      Because multi vulkan instance may crush your applications. */
    std::unique_ptr<Instance> instance;

    /*! \brief The physical device (aka gpu). */
    std::unique_ptr<PhysicalDevice> physicalDevice;

    /*! \brief The logical device of this render context. */
    std::unique_ptr<LogicalDevice> device;

    /*! \brief The render session container.
      Instead of let the render context to control everything, I use render
     sessions for muiti window(surface) management. There should be more
     sessions. */
    std::vector<std::unique_ptr<RenderSession>> sessions;
};
} // namespace cruelRender
} // namespace cruelEngine