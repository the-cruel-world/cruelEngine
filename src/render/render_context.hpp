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

/**
 * \brief RenderProp is the brief detailes/settings required to create a RenderContext.
 *
 * To create a RenderContext, we will need to set many parameters, so I put all these parameters
 * togather to make the creation of Contexts cleaner.
 *
 * \todo
 * 1. hello world
 */
struct RenderProp
{
    //! appInfo is the vulkan application creation information required by the creation of vulkan
    //! instance.
    VkApplicationInfo appInfo = {.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                 .pApplicationName = "Cruel Engine",
                                 .pEngineName      = "Cruel Engine",
                                 .apiVersion       = USED_VULKAN_API_VERSION};
    //! Whether we need to enable validation layers or not. We enable validation layers during the
    //! development stages.
    bool validation = true;

    //! The names of validation layers in this vector will be enabled for this RenderContext.

    std::vector<const char *> validationLayers = {
        "VK_LAYER_KHRONOS_validation",
    };

    //! The names of instance extensions in this vector will be enabled for this RenderContext.
    std::vector<const char *> enabledInstanceExtensions = {};

    //! The names of device extensions in this vector will be enabled for this RenderContext.
    std::vector<const char *> enabledDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    //! The device features that required by this RenderContext, it's also used to select physicale
    //! devices.
    VkPhysicalDeviceFeatures features = {};

    //! The type of queues required by the RenderContext.
    VkQueueFlags flags = {VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_COMPUTE_BIT};

    //! Enable overlay or not. Not supported currently.
    bool overlay = false;
};

/**
 * \brief RenderContext is the most basic target of this rendering API.
 *
 * It's the context of a Vulcan instance for rendering usage. It's a necessary part for an
 * application to render something on GPU.  It will create an Vulcan instance, enumerate  all GPU
 * devices, and pick the proper once if possible. Of course, you can also pick the best GPU
 * manually.
 */
class RenderContext
{
public:
    RenderContext(RenderProp properties);

    ~RenderContext();

    /** Tasks to prepare/create vulkan objects for RenderContext. */
    void prepareRender();

    /** \todo
     * 1. remove.
     * 2. draw will record all the draw commands for all render sessions. it's not a good
     * design. the render sessions should manage draw recordings by theirselves.
     * */
    void draw();

    /** \todo
     * 1. remove.
     * 2. render_frame will send signal to every RenderSession`s to submit and present a new
     * frame. It's not a good design. the render sessions should manage draw recordings
     * by theirselves.
     * */
    void render_frame();

    //! Get the handle of vulkan instance
    Instance &get_instance() const;

    //! Get the gpu object.
    PhysicalDevice &get_gpu() const;

    //! Get the logical device.
    LogicalDevice &get_device() const;

    //! Get the RenderSession instance with specific session index.
    RenderSession &get_session(u32 index) const;

    //! Add a new RenderSession instance
    void add_session(std::string session_name, SessionProp &properties);

    //! Ask whether the RenderContext should be destroied or not.
    bool is_context_alive();

private:
    /** \brief the brief detailes/settings required to create a RenderContext. */
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