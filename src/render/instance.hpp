#pragma once

#include "../common.h"
#include "../types.h"
#include "../window/window.hpp"
#include "../debug/debug.hpp"

namespace realEngine {
    class Instance {
    public: 
        Instance(const Window &_theWindow, const VkApplicationInfo &_appInfo, const bool _validation,
            const std::vector<const char*> _validationLayers,
            const std::vector<const char*> _instanceExtensions) :
            topWindow(_theWindow), appInfo(_appInfo), validation(_validation), validationLayers(_validationLayers), requiredInstanceExtensions(_instanceExtensions)
        {
            createInstance();
            VK_CHECK_RESULT(glfwCreateWindowSurface(instance, topWindow.window, nullptr, &surface));
        }
        virtual ~Instance(){
            //std::cout << "clean up instance" << std::endl;
            vkDestroySurfaceKHR(instance, surface, nullptr);
            vkDestroyInstance(instance, nullptr);
        }

        VkSurfaceKHR    surface;
        VkInstance      instance;
        void            createInstance();

        const Window    &topWindow; // Use glfwWindow or Window???
        const VkApplicationInfo &appInfo;
        const bool      validation;
        const std::vector<const char*>
                        validationLayers;

        const std::vector<const char*>
                        requiredInstanceExtensions;
        std::vector<const char*> getRequiredInstanceExtensions();

    private:
    };
}