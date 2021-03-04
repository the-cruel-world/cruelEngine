#pragma once
#include "vkcommon.h"

namespace cruelEngine {
    class Window;
namespace VulkanContext {
    class Instance {
    public: 
        Instance(const Window &_theWindow, const VkApplicationInfo &_appInfo, const bool _validation,
            const std::vector<const char*> _validationLayers,
            const std::vector<const char*> _instanceExtensions);

        Instance(const Instance&) = delete;

        Instance(Instance &&) = delete;

        Instance operator=(const Instance&) = delete;

        Instance operator=(Instance&&) = delete;

        virtual ~Instance(){
            //std::cout << "clean up instance" << std::endl;
            vkDestroySurfaceKHR(handle, surface, nullptr);
            vkDestroyInstance(handle, nullptr);
        }

        void            query_gpus();

        const std::vector<VkPhysicalDevice> &get_gpus() const {return gpus;}

        const VkSurfaceKHR&     get_surface() const {return surface;}

        const VkInstance&       get_handle() const {return handle;}

        const Window&           get_window() const {return topWindow;}

    private:
        VkSurfaceKHR    surface;
        VkInstance      handle;
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
        std::vector<VkPhysicalDevice> gpus;
    };
}
}