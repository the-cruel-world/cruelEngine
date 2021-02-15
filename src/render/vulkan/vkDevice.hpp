#pragma once

#include "vkcommon.h"

namespace cruelEngine {
    const VkPhysicalDeviceFeatures noFeature = {};
    struct VkDeviceQueueFamilyIndex {
        i32 graphics = -1;
        i32 present = -1;
        i32 compute = -1;
        i32 transfer = -1;
        VkDeviceQueueFamilyIndex() {};
        VkDeviceQueueFamilyIndex(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, VkQueueFlags& flags);

        void findIndices(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, VkQueueFlags& flags);
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

        VkDevice            logicalDevice = VK_NULL_HANDLE;

        VkPhysicalDevice    physicalDevice = VK_NULL_HANDLE;
        std::vector<VkPhysicalDevice> suitablePhysicalDevices; 

        void                selectPhysicalDevice(int order = 0);
        void                createLogicalDevice();

        VkQueue             getGraphicsQueue() const {return graphic;}
        VkQueue             getComputeQueue() const {return compute;}
        VkQueue             getPresentQueue() const {return present;}

        u32                 getGraphicsQueueIndex() const {return queueFamilyInices.graphics;}
        u32                 getComputeQueueIndex() const {return queueFamilyInices.compute;}
        u32                 getPresentQueueIndex() const {return queueFamilyInices.present;}

    private:
        VkQueue             graphic;
        VkQueue             compute;
        VkQueue             present;

        VkDeviceQueueFamilyIndex queueFamilyInices;

        void                getSuitablePhysicalDevices();

        void                pickPhysicalDevice();
        virtual bool        isDeviceSuitable(VkPhysicalDevice device);
        bool                extensionSupported();

        void                destroyLogicalDevice();

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