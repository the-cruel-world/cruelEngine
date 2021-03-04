#include "render_context.hpp"
#include "render_task.hpp"
#include "render_session.hpp"

#include "vulkan/logical_device.hpp"
#include "vulkan/physical_device.hpp"
#include "vulkan/instance.hpp"
#include "vulkan/swapchain.hpp"

#include "../window/window.hpp"

namespace cruelEngine {
namespace cruelRender {

    RenderContext::RenderContext ()
    {
        VkPhysicalDeviceFeatures features {};

        std::vector<const char*> requiredDeviceExtensions {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

        std::vector<const char*> instanceExtensions {VK_KHR_SURFACE_EXTENSION_NAME};

        std::vector<const char*> layernames {"VK_LAYER_KHRONOS_validation", "VK_LAYER_LUNARG_monitor"};

        VkQueueFlags flags {VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_COMPUTE_BIT};

        // create Instance.
        glfwInit();
        instance = std::make_unique<Instance>(appInfo, true, layernames, instanceExtensions);
        glfwTerminate();

        std::cout << "[RenderContext] Instance created" << std::endl;
        for (auto &gpu : instance->get_gpus())
        {
            if (isPhysicalDeviceSuitable(gpu, features, requiredDeviceExtensions, flags))
            {
                // create Physical device
                physicalDevice = std::make_unique<PhysicalDevice>(gpu, features);
            }
        }
        std::cout << "[RenderContext] Physical device created" << std::endl;
        // Create Logical device
        device = std::make_unique<LogicalDevice>(*physicalDevice, layernames, requiredDeviceExtensions, flags);
        std::cout << "[RenderContext] Logical device created" << std::endl;

        RenderProp  properties = {true, {"VK_LAYER_KHRONOS_validation"}, {}, {VK_KHR_SWAPCHAIN_EXTENSION_NAME}, false, false};

        // sessions.push_back(std::make_unique<RenderSession>(*instance, *device, properties));
    }

    void RenderContext::prepareRender()
    {

    }

    RenderContext::~RenderContext()
    {
        sessions.clear();

        device.reset();

        physicalDevice.reset();

        instance.reset();
    }

    RenderSession& RenderContext::get_session(u32 index) const 
    {
        if (index < sessions.size())
            return *(sessions[index]);
        else 
            return *(sessions[0]);
    }

    void RenderContext::add_session(std::string session_name, RenderProp &properties)
    {
        sessions.push_back(std::make_unique<RenderSession>(*instance, *device, properties));
        std::cout << "[RenderContext] Render Session created : " << session_name << std::endl;
    }

    void RenderContext::draw()
    {
        for (auto &session: sessions)
        {
            session->draw();
        }
    }

    void RenderContext::render_frame()
    {
        for (auto &session: sessions)
        {
            session->render_frame();
        }
    }

    bool RenderContext::is_context_alive()
    {
        for (auto &session: sessions)
        {
            if (session->is_session_alive())
                return true;
        }
        return false;
    }

    // void RenderContext::loadObject(const cruelEngine::cruelScene::Object &theObject)
    // {

    // }

    // void RenderContext::loadScene(const cruelEngine::cruelScene::Scene &theScene)
    // {

    // }
}
}