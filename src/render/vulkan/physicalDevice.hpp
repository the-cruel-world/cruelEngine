#pragma once

#include "vkcommon.h"

#include "instance.hpp"

namespace cruelEngine {
namespace VulkanContext {

    std::vector<VkPhysicalDevice> & getPhysicalDevices(const Instance &instance);

    std::vector<VkPhysicalDevice> & getSuitablePhysicalDevices(const Instance &instance, const VkPhysicalDeviceFeatures &requiredFeatures, const std::vector<const char*> requiredExtensions);

    bool isPhysicalDeviceSuitable (const VkPhysicalDevice & physicalDevice, const VkPhysicalDeviceFeatures &requiredFeatures, const std::vector<const char*> requiredExtensions);

    struct VkDeviceQueueFamilyIndex {
        i32 graphics = -1;
        i32 present = -1;
        i32 compute = -1;
        i32 transfer = -1;
        VkDeviceQueueFamilyIndex() {};
        VkDeviceQueueFamilyIndex(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, VkQueueFlags& flags);

        void findIndices(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, VkQueueFlags& flags);
    };

    struct PhysicalDevice {
    public:
        PhysicalDevice(const VkPhysicalDevice& physicalDevice);
        
        u32 getMemotyTypeIndex(const u32& filter, const VkMemoryPropertyFlags& flags) const;

        const VkPhysicalDeviceFeatures& getDeviceFeatures() const {return features;}

        const VkPhysicalDeviceMemoryProperties& getMemotyProperties() const {return memoryProperties;}

        const VkSampleCountFlagBits& getMultiSamplingLevel() const;

    private:

        VkPhysicalDevice            physicalDevice;

        VkPhysicalDeviceFeatures    features{};

        // The GPU properties
        VkPhysicalDeviceProperties  properties;

        // The GPU memory properties
        VkPhysicalDeviceMemoryProperties memoryProperties;

        // The GPU queue family properties
        std::vector<VkQueueFamilyProperties> queueFamilyProperties;

        // The depthFormat
        VkFormat                    depthFormat;

        // Holds the extension feature structures, we use a map to retain an order of requested structures
        std::vector<const char *>   extension_features;
    };
}
}