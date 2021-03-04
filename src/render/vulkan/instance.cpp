#include "instance.hpp"
#include "../../window/window.hpp"

namespace cruelEngine {
namespace cruelRender {

    Instance::Instance(const VkApplicationInfo &_appInfo, const bool _validation,
        const std::vector<const char*> _validationLayers,
        const std::vector<const char*> _instanceExtensions) :
        appInfo(_appInfo), validation(_validation), validationLayers(_validationLayers), requiredInstanceExtensions(_instanceExtensions)
    {
        std::vector<const char*> instanceExtensions = getRequiredInstanceExtensions();
        VkInstanceCreateInfo instCreateInfo = {};
        instCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instCreateInfo.pNext = NULL;
        instCreateInfo.pApplicationInfo = &appInfo;

        instCreateInfo.enabledExtensionCount = static_cast<u32> (instanceExtensions.size());
        instCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

        if (validation) {
            instCreateInfo.enabledLayerCount = static_cast<u32> (validationLayers.size());
            instCreateInfo.ppEnabledLayerNames = validationLayers.data();
        } else 
            instCreateInfo.enabledLayerCount = 0;
        VK_CHECK_RESULT(vkCreateInstance(&instCreateInfo, nullptr, &handle));

        query_gpus();
    }

    Instance::~Instance()
    {
        if (handle != VK_NULL_HANDLE)
            vkDestroyInstance(handle, nullptr);
    }
    
    std::vector<const char*> 
    Instance::getRequiredInstanceExtensions()
    {
        std::vector<const char*> extensions;

        unsigned int glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        for (unsigned int i = 0; i < glfwExtensionCount; i++) {
            extensions.push_back(glfwExtensions[i]);
        }
        if (requiredInstanceExtensions.size() > 0) {
            for (const char * enabled: requiredInstanceExtensions)
                extensions.push_back(enabled);
        }
        return extensions;
    }

    void Instance::query_gpus()
    {
        u32 deviceCount = 0;
        vkEnumeratePhysicalDevices(handle, &deviceCount, nullptr);

        if (!deviceCount)
            throw std::runtime_error("Failed to find GPUS with Vulkan support.");

        std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
        vkEnumeratePhysicalDevices(handle, &deviceCount, physicalDevices.data());

        for (const auto &physicalDev: physicalDevices) {
            gpus.push_back(physicalDev);
        }
    }
}
}