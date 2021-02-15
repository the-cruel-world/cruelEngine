#include "instance.hpp"

namespace cruelEngine
{

    Instance::Instance(const Window &_theWindow, const VkApplicationInfo &_appInfo, const bool _validation,
        const std::vector<const char*> _validationLayers,
        const std::vector<const char*> _instanceExtensions) :
        topWindow(_theWindow), appInfo(_appInfo), validation(_validation), validationLayers(_validationLayers), requiredInstanceExtensions(_instanceExtensions)
    {
        createInstance();
        VK_CHECK_RESULT(glfwCreateWindowSurface(instance, topWindow.window, nullptr, &surface));
    }
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
} // namespace cruelEngine
