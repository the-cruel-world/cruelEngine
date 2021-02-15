#pragma once

#include "vkcommon.h"

#include "instance.hpp"


namespace cruelEngine
{
    class PhysicalDevice {
    public:
        PhysicalDevice(Instance &instance, VkPhysicalDevice physical_device);

    private:
        Instance &instance;

        VkPhysicalDeviceFeatures features{};

        // The GPU properties
        VkPhysicalDeviceProperties properties;

        // The GPU memory properties
        VkPhysicalDeviceMemoryProperties memoryProperties;

        // The GPU queue family properties
        std::vector<VkQueueFamilyProperties> queueFamilyProperties;

        // The features that will be requested to be enabled in the logical device
        VkPhysicalDeviceFeatures requiredFeatures{};

        // The extension feature pointer
        std::vector<const char *> requiredExtensions{};

        // Holds the extension feature structures, we use a map to retain an order of requested structures
        // std::map<VkStructureType, std::shared_ptr<void>> extension_features;
    };
} // namespace cruelEngine
 