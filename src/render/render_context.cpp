#include "render_context.hpp"

#include "../window/window.hpp"
#include "render_session.hpp"
#include "render_task.hpp"
#include "vulkan/instance.hpp"
#include "vulkan/logical_device.hpp"
#include "vulkan/physical_device.hpp"
#include "vulkan/swapchain.hpp"

namespace cruelEngine::cruelRender
{
RenderContext::RenderContext(RenderProp properties) : properties{properties}
{
    /** \brief create Instance.
     * Running glfwInit() before creating instance, because glfw requires extra instance extensions.
     * Only after glfwInit(), the instance can find the proper glfw extensions. Maybe I need to put
     * it into constructor of the instance.
     * */
    glfwInit();
    instance = std::make_unique<Instance>(properties.appInfo, properties.validation,
                                          properties.validationLayers,
                                          properties.enabledInstanceExtensions);
    glfwTerminate();
#ifdef RENDER_DEBUG
    std::cout << "[RenderContext] Instance created" << std::endl;
#endif

    // pick suitable physical device aka gpu.
    for (auto &gpu : instance->get_gpus())
    {
        if (isPhysicalDeviceSuitable(gpu, properties.features, properties.enabledDeviceExtensions,
                                     properties.flags))
        {
            physicalDevice = std::make_unique<PhysicalDevice>(gpu, properties.features);
        }
    }
#ifdef RENDER_DEBUG
    std::cout << "[RenderContext] Physical device created" << std::endl;
#endif

    // Create Logical device
    device = std::make_unique<LogicalDevice>(*physicalDevice, properties.validationLayers,
                                             properties.enabledDeviceExtensions, properties.flags);
#ifdef RENDER_DEBUG
    std::cout << "[RenderContext] Logical device created" << std::endl;
#endif
}

void RenderContext::prepareRender()
{}

RenderContext::~RenderContext()
{
    /* Destroy all sessions before the device. */
    sessions.clear();

    /* Release the logical device. */
    device.reset();

    /* Clear all gpu candidates. */
    physicalDevice.reset();

    /* Destroy instance. */
    instance.reset();
}

RenderSession &RenderContext::get_session(u32 index) const
{
    if (index < sessions.size())
        return *(sessions[index]);
    else
    {
#ifdef RENDER_DEBUG
        std::cout << "[RenderContext] Get render session: out of range" << std::endl;
#endif
        return *(sessions[0]);
    }
}

void RenderContext::add_session(std::string session_name, SessionProp &properties)
{
    u32 id = sessions.size();
    sessions.push_back(std::make_unique<RenderSession>(*instance, *device, properties));
    sessions.back()->set_session_id(id);
#ifdef RENDER_DEBUG
    std::cout << "[RenderContext] Render Session created : " << session_name
              << "\tid:" << sessions.back()->get_session_id() << std::endl;
#endif
}

void RenderContext::draw()
{
    for (auto &session : sessions)
    {
        session->draw();
    }
}

/**
 * Need Multi thread!
 */
void RenderContext::render_frame()
{
    RENDER_LOG("Session num: %d\n", static_cast<int>(sessions.size()));
    for (auto &session : sessions)
    {
        if (session->is_session_alive())
            session->render_frame();
    }
}

Instance &RenderContext::get_instance() const
{
    return *instance;
}

PhysicalDevice &RenderContext::get_gpu() const
{
    return *physicalDevice;
}

LogicalDevice &RenderContext::get_device() const
{
    return *device;
}

bool RenderContext::is_context_alive()
{
    sessions.erase(std::remove_if(sessions.begin(), sessions.end(),
                                  [](std::unique_ptr<RenderSession> const &session) {
                                      return !session->is_session_alive();
                                  }),
                   sessions.end());
    return (sessions.size() > 0);
}

// void RenderContext::loadObject(const cruelEngine::cruelScene::Object
// &theObject)
// {

// }

// void RenderContext::loadScene(const cruelEngine::cruelScene::Scene &theScene)
// {

// }

} // namespace cruelEngine::cruelRender