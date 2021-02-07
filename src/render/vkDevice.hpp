#pragma once

#include "../common.h"
#include "../types.h"
#include "../debug/debug.hpp"

namespace realEngine {
    const VkPhysicalDeviceFeatures noFeature = {};
    struct VkDeviceQueueFamilyIndex {
        i32 graphics = -1;
        i32 present = -1;
        i32 compute = -1;
        i32 transfer = -1;
        VkDeviceQueueFamilyIndex(const VkPhysicalDevice& _physical, const VkSurfaceKHR& _surface, VkQueueFlags& _flags)
        : physicalDevice(_physical), surface(_surface), flags(_flags)
        {
            findIndices();
        }
    protected:
        const VkPhysicalDevice  &physicalDevice;
        const VkSurfaceKHR      &surface;
        const VkQueueFlags      &flags; 
        void                    findIndices();
    };

    class VulkanDevice
    {
    public:
        VulkanDevice(bool _validataonLayer, 
            const std::vector<const char*> &_layerNames, 
            const VkInstance &_instance, const VkSurfaceKHR &_surface,
            const std::vector<const char*> &_requiredExtensions,
            const VkPhysicalDeviceFeatures &_requiredFeatures = noFeature);

        virtual ~VulkanDevice();

        VkDevice            logicalDevice;

        VkPhysicalDevice    physicalDevice;
        std::vector<VkPhysicalDevice> suitablePhysicalDevices; 

        void                selectPhysicalDevice(int order = 0);
        void                createLogicalDevice();

        VkQueue             getGraphicsQueue() {return graphic;}
        VkQueue             getComputeQueue() {return compute;}
        VkQueue             getPresentQueue() {return present;}

    private:
        VkQueue             graphic;
        VkQueue             compute;
        VkQueue             present;

        void                getSuitablePhysicalDevices();

        void                pickPhysicalDevice();
        virtual bool        isDeviceSuitable(VkPhysicalDevice device);
        bool                extensionSupported();

        void                destroyLoginalDevice();

        VkQueueFlags        flags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_COMPUTE_BIT;
        const bool          validationLayer;// 
        std::vector<const char*> layerNames;//
        const VkInstance    &instance;      // Reference to main Vulkan instance
        const VkSurfaceKHR  &surface;       // reference to main Vk surface;
        std::vector<const char*> requiredExtensions;
        const VkPhysicalDeviceFeatures &requiredFeatures;    
    };
}


/// Very old code 
// struct VulkanDevice {
//     VkPhysicalDevice    physicalDevice;
//     VkDevice            logicalDevice;

//     VkPhysicalDeviceProperties      properties;
//     VkPhysicalDeviceFeatures        features;
//     VkPhysicalDeviceFeatures        enabledFeatures;
//     VkPhysicalDeviceMemoryProperties memoryProperties;
//     std::vector<VkQueueFamilyProperties> queueFamilyperporties;
//     std::vector<std::string>        supportedExtensions;
//     VkCommandPool                   commandPool = VK_NULL_HANDLE;

//     bool                enableDebugMarkers = false;

//     struct {
//         u32 graphics;
//         u32 compute;
//         u32 transfer;
//     } queueFamilyInices;

//     explicit VulkanDevice(VkPhysicalDevice physicalDev);
//     ~VulkanDevice();

//     u32                 getQueueFamilyIndex(VkQueueFlagBits queueFlags) const;
//     bool                extensionSupported(std::string extension);
//     void                createCommandPool(u32 queueFamilyIndex, VkCommandPoolCreateFlags createFlags = 
//         VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

//     VkResult            createLogicalDevice(VkPhysicalDeviceFeatures enabledFeatures,
//         std::vector<const char*> enabledExtensions, void *nextChain, bool useSwapChain = true, 
//         VkQueueFlags requestedQueueTypes = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);

// };