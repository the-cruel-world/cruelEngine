#pragma once

#include "vkcommon.h"

#include "instance.hpp"
#include "vkDevice.hpp"
#include "swapChain.hpp"
#include "pipeLine.hpp"
#include "renderPass.hpp"


#define MAIN_VER 0
#define MID_VER  0
#define END_VER  1

// class Engine {
//     public:
//         Engine();
//         virtual ~Engine();
//         void    run();

//     private:
//         u32             REAL_ENGINE_VERSION = VK_MAKE_VERSION(MAIN_VER, MID_VER, END_VER);
    
//     public:

//         GLFWwindow      *mainWindow = nullptr;
//         int             windowHeight = 720;
//         int             windowWidth = 1280;
//         void            initWindow();
//         void            destroyWindow();

//         VkSurfaceKHR    surface;
//         virtual void    createWindowSurface();
//         virtual void    destroyWindowSurface();


//         struct Settings {
//             bool        validation = true;
//             const std::vector<const char*>
//                         validationLayers = {
//                             "VK_LAYER_KHRONOS_validation",
//                         };
//             const std::vector<const char*>
//                         enabledInstanceExtensions = {};
//             const std::vector<const char*>
//                         enabledDeviceExtensions = {};
//             bool        fullscreen = false;

//             bool        vsync = false;
//             bool        overlay = false;
//         } settings;

//         //Camera          camera;
//         glm::vec2       mousePos;

//         std::string     appName = "Demo";
//         std::string     title = "Engine";
//         std::string     name = "Real Engine";

//         u32             vkApiVersion = VK_API_VERSION_1_0;

//         void            initVulkan();

//         VkInstance      vkInstance;
//         virtual void    createInstance();
//         virtual void    destroyInstance();

//         //virtual int     render();
//         //virtual void    viewChanged();
//         //virtual void    keyPressed();
//         //virtual void    moseMoved();
//         //virtual void    windowResized();

//         //virtual int     createCommandBuffers();
//         //virtual int     setDetphStencil();
//         //virtual int     setFrameBuffers();
//         //virtual int     setRenderPass();

//         //virtual int     getEnabledFearures();
//         std::vector<const char*>
//                         getRequiredInstanceExtensions();

//         //virtual int     setResource();

//         //int             drawUI();
//         //int             prepareFrame();
//         //int             submitFrame();
//         //virtual int     renderFrame();

//         //virtual void    onUpdateUIOverlay(UIOverlay *overlay);
        
//         void            renderLoop();
//         VkPhysicalDevice physicalDevice;
//         void            pickPhysicalDevice();

//         virtual bool    isDeviceSuitable(VkPhysicalDevice device);

//         VulkanDevice    *vulkanDevice = nullptr;
//         VkDevice        device;
//         void            * deviceCreatepNextChain = nullptr;
//         void            createLogicalDevice();
//         void            destroyLogicalDevice();
// };


namespace cruelEngine {

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