#pragma once

#include "vkcommon.h"

#include "instance.hpp"
#include "vkDevice.hpp"
#include "swapChain.hpp"
#include "pipeLine.hpp"
#include "renderPass.hpp"


#define MAIN_VER 0
#define MID_VER  0
#define END_VER  1Z

namespace cruelEngine {
namespace VulkanContext {
    class VulkanContext : RenderContext
    {
    public: 
        VulkanContext () : RenderContext() 
        {

        }

        virtual ~VulkanContext() {}
    };

    struct RenderProp {
            bool        validation = true;

            const std::vector<const char*>
                        validationLayers = {};
            const std::vector<const char*>
                        enabledInstanceExtensions = {};

            const std::vector<const char*>
                        enabledDeviceExtensions = {};

            bool        vsync = false;
            bool        overlay = false;
    };

    class Render {
    public:
        Render(const Window &_topwindow, const VkApplicationInfo &_appInfo, const RenderProp &_settings,
            const Instance &_instance, const VulkanDevice &_device)
                : topWindow(_topwindow), appInfo(_appInfo), settings(_settings),
                instance(_instance), device(_device)
        {
            
        }

        virtual ~Render(){

        }

        // The window that this render will present 
        const Window            &topWindow;
        // AppInfo 
        const VkApplicationInfo &appInfo;

        const RenderProp        &settings;

        /// I dont wnat to use pointer here, need some change.
        // The Vulkan Instance 
        const Instance          &instance;
        // The Vulkan Devices physical and logical
        const VulkanDevice      &device;
    };
}
}