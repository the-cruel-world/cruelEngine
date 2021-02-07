#include "instance.hpp"

namespace realEngine
{
    void Instance::createInstance()
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
        VK_CHECK_RESULT(vkCreateInstance(&instCreateInfo, nullptr, &instance));
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
} // namespace realEngine
