#include "render.hpp"
#include "logical_device.hpp"
#include "physical_device.hpp"
#include "instance.hpp"
#include "swapchain.hpp"
#include "render_task.hpp"
#include "render_session.hpp"

#include "../../window/window.hpp"
#include "../render_context.hpp"

namespace cruelEngine {
namespace VulkanContext {

    VulkanContext::VulkanContext (const Window& _window) : RenderContext(_window)
    {
        VkPhysicalDeviceFeatures features {};
        std::vector<const char*> requiredDeviceExtensions {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
        std::vector<const char*> instanceExtensions {};
        std::vector<const char*> layernames {"VK_LAYER_KHRONOS_validation"};
        VkQueueFlags flags {VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_COMPUTE_BIT};

        // create Instance.
        instance = std::make_unique<Instance>(mainWindow, appInfo, true, layernames, instanceExtensions);

        std::cout << "[VulkanContext] Instance created" << std::endl;
        for (auto &gpu : instance->get_gpus())
        {
            if (isPhysicalDeviceSuitable(gpu, instance->get_surface(), features, requiredDeviceExtensions, flags))
            {
                // create Physical device
                physicalDevice = std::make_unique<PhysicalDevice>(gpu, features);
            }
        }
        std::cout << "[VulkanContext] Physical device created" << std::endl;
        // Create Logical device
        device = std::make_unique<LogicalDevice>(*physicalDevice, instance->get_surface(), layernames,
            requiredDeviceExtensions, flags);
        std::cout << "[VulkanContext] Logical device created" << std::endl;

        RenderProp  properties = {true, {"VK_LAYER_KHRONOS_validation"}, {}, {VK_KHR_SWAPCHAIN_EXTENSION_NAME}, false, false};

        session = std::make_unique<RenderSession>(*instance, *device, properties);
        std::cout << "[VulkanContext] Render Session created" << std::endl;
    }

    void VulkanContext::prepareRender()
    {

    }

    VulkanContext::~VulkanContext()
    {
        session.reset();

        device.reset();

        physicalDevice.reset();

        instance.reset();
    }

    void VulkanContext::draw()
    {
        session->draw();
    }

    void VulkanContext::render_frame()
    {
        session->render_frame();
    }

    // void VulkanContext::loadObject(const cruelEngine::cruelScene::Object &theObject)
    // {

    // }

    // void VulkanContext::loadScene(const cruelEngine::cruelScene::Scene &theScene)
    // {

    // }
}
}