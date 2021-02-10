#include "vkDevice.hpp"

namespace realEngine {

    VkDeviceQueueFamilyIndex::VkDeviceQueueFamilyIndex(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, VkQueueFlags& flags)
    {
        findIndices(physicalDevice, surface, flags);
    }
    
    void VkDeviceQueueFamilyIndex::findIndices(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, VkQueueFlags& flags)
    {
        u32 queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        // Find graphic queue & present queue
        if (flags & VK_QUEUE_GRAPHICS_BIT) {
            for (u32 i = 0; i < queueFamilyCount; ++i)
            {
                const auto &queueFamily = queueFamilies[i];

                if (queueFamily.queueCount > 0 && graphics < 0 && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT))
                {
                    //std::cout << "Got Graphic!" << std::endl;
                    graphics = i;
                    break;
                }
            }
            for (u32 i = 0; i < queueFamilyCount; ++i)
            {
                const auto &queueFamily = queueFamilies[i];

                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, i, surface, &presentSupport);
                // if (!presentSupport)
                //     std::cout << "No present!" << std::endl;
                if (queueFamily.queueCount > 0 && present < 0 && presentSupport)
                {
                    present = i;
                    break;
                }
            }
        }
        // Find compute queue
        if (flags & VK_QUEUE_COMPUTE_BIT){
            for (int i = 0; i < queueFamilies.size(); ++i)
            {
                const auto &queueFamily = queueFamilies[i];

                if (queueFamily.queueCount > 0 &&
                    compute < 0 &&
                    i != graphics && i != present &&
                    (queueFamily.queueCount & VK_QUEUE_COMPUTE_BIT))
                {
                    compute = i;
                    break;
                }
            }

            // fall back to using graphcis queue if no dedicated comptue queue is available
            if (compute < 0)
                compute = graphics;
        }
        if (flags & VK_QUEUE_TRANSFER_BIT)
        {
            for (int i = 0; i < queueFamilies.size(); ++i)
            {
                const auto &queueFamily = queueFamilies[i];

                if (queueFamily.queueCount > 0 &&
                    transfer < 0 &&
                    i != graphics && i != present && i != compute &&
                    (queueFamily.queueCount & VK_QUEUE_TRANSFER_BIT))
                {
                    transfer = i;
                    break;
                }
            }

            if (transfer < 0)
                transfer = graphics < 0 ? compute : graphics;
        }
    }

    VulkanDevice::VulkanDevice(bool _validataonLayer, 
            const std::vector<const char*> &_layerNames,
            const VkInstance &_instance, const VkSurfaceKHR &_surface,
            const std::vector<const char*> &_requiredExtensions,
            const VkPhysicalDeviceFeatures &_requiredFeatures): validationLayer(_validataonLayer), layerNames(_layerNames), 
                instance(_instance), surface(_surface), requiredExtensions(_requiredExtensions), requiredFeatures(_requiredFeatures)
    {
        getSuitablePhysicalDevices();
    }

    /* Create device
        1. Get all physical device. 
        2. Select a nice device.
        3. Create a logical device.
    */ 

    /* Create devices 
        1. create a instence of VulkanDevice
        2. select a suitable physical device 
        3. create a logical device
    */

    VulkanDevice::~VulkanDevice(){ destroyLogicalDevice(); }

    void VulkanDevice::getSuitablePhysicalDevices()
    {
        u32 deviceCount = 0;
        vkEnumeratePhysicalDevices(this->instance, &deviceCount, nullptr);

        if (!deviceCount)
            throw std::runtime_error("Failed to find GPUS with Vulkan support.");

        std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
        vkEnumeratePhysicalDevices(this->instance, &deviceCount, physicalDevices.data());

        for (const auto &physicalDev: physicalDevices) {
            if (isDeviceSuitable(physicalDev)) {
                suitablePhysicalDevices.push_back(physicalDev);
            }
        }

        if (suitablePhysicalDevices.empty()) {
            throw std::runtime_error("No suitable GPU found!");
        }
    }

    void VulkanDevice::selectPhysicalDevice(int order) {
        if (order < suitablePhysicalDevices.size())
            physicalDevice = suitablePhysicalDevices[order];
        else {
            physicalDevice = suitablePhysicalDevices[0];
            std::cerr << "Select suitable physical device: index "<<order<<" out of range[0,"
                <<suitablePhysicalDevices.size()-1<<"], use 0 instead." << std::endl;
        }
    }

    bool VulkanDevice::isDeviceSuitable(VkPhysicalDevice device)
    {
        // Extension support check
        bool extensionSupported = true;
        std::vector<std::string> supportedExtensions = {};
        u32 extCount = 0;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount, nullptr);
        if (extCount > 0){
            std::vector<VkExtensionProperties> extensions(extCount);
            if (vkEnumerateDeviceExtensionProperties(device, nullptr, &extCount, &extensions.front()) == VK_SUCCESS){
                for (auto ext: extensions){
                supportedExtensions.push_back(ext.extensionName);
                //std::cout << ext.extensionName << std::endl;
                }
            }
        }
        for (const auto &requiredExtension:requiredExtensions){
            //std::cout << "Required: " << requiredExtension << std::endl;
            if (find (supportedExtensions.begin(), supportedExtensions.end(), requiredExtension) == supportedExtensions.end()){
                extensionSupported = false;
                break;
            }
        }
                
        bool queueSupported = true;

        VkDeviceQueueFamilyIndex index(device, surface, flags);

        //std::cout << "Flags: " << index.present << ",\t" << index.graphics << ",\t" << index.compute << std::endl;
        if (!(flags & VK_QUEUE_GRAPHICS_BIT) || index.graphics < 0 || index.present <0)
            queueSupported = false;
        if (!(flags & VK_QUEUE_COMPUTE_BIT) || index.compute < 0)
            queueSupported = false;
        if (!(flags & VK_QUEUE_TRANSFER_BIT) || index.transfer < 0)
           queueSupported = false;
        return queueSupported && extensionSupported;
    }

    bool VulkanDevice::extensionSupported()
    {
        return false;
    }

    void VulkanDevice::createLogicalDevice()
    {

        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
        const float defaultQueuePriority = 1.0f;

        queueFamilyInices.findIndices(physicalDevice, surface, flags);
        
        // Set queue requirements
        if (flags & VK_QUEUE_GRAPHICS_BIT) {
            VkDeviceQueueCreateInfo queueInfo{};
            queueInfo.sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueInfo.queueFamilyIndex  = queueFamilyInices.graphics;
            queueInfo.queueCount        = 1;
            queueInfo.pQueuePriorities  = &defaultQueuePriority;
            queueCreateInfos.push_back(queueInfo);
            if (queueFamilyInices.present != queueFamilyInices.graphics){
                VkDeviceQueueCreateInfo queueInfo{};
                queueInfo.sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueInfo.queueFamilyIndex  = queueFamilyInices.present;
                queueInfo.queueCount        = 1;
                queueInfo.pQueuePriorities  = &defaultQueuePriority;
                queueCreateInfos.push_back(queueInfo);
            }
        } else 
            queueFamilyInices.graphics = VK_NULL_HANDLE;
        if (flags & VK_QUEUE_COMPUTE_BIT) {
            if ((queueFamilyInices.compute != queueFamilyInices.graphics) && (queueFamilyInices.compute != queueFamilyInices.present)){
                VkDeviceQueueCreateInfo queueInfo{};

                queueInfo.sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueInfo.queueFamilyIndex  = queueFamilyInices.compute;
                queueInfo.queueCount        = 1;
                queueInfo.pQueuePriorities  = &defaultQueuePriority;
                queueCreateInfos.push_back(queueInfo);   
            }
        } else 
            queueFamilyInices.compute = queueFamilyInices.graphics;
        if (flags & VK_QUEUE_TRANSFER_BIT){
            if ((queueFamilyInices.transfer != queueFamilyInices.graphics) && (queueFamilyInices.transfer != queueFamilyInices.present) && (queueFamilyInices.transfer != queueFamilyInices.compute)) {
                VkDeviceQueueCreateInfo queueInfo{};

                queueInfo.sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
                queueInfo.queueFamilyIndex  = queueFamilyInices.transfer;
                queueInfo.queueCount        = 1;
                queueInfo.pQueuePriorities  = &defaultQueuePriority;
                queueCreateInfos.push_back(queueInfo);
            }
        } else
            queueFamilyInices.transfer = queueFamilyInices.graphics;

        VkDeviceCreateInfo deviceCreateInfo = {};
        deviceCreateInfo.sType              = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.queueCreateInfoCount   = static_cast<u32>(queueCreateInfos.size());
        deviceCreateInfo.pQueueCreateInfos  = queueCreateInfos.data();
        deviceCreateInfo.pEnabledFeatures   = &requiredFeatures;

        deviceCreateInfo.enabledExtensionCount = static_cast<u32>(requiredExtensions.size());
        deviceCreateInfo.ppEnabledExtensionNames = requiredExtensions.data();

        if (validationLayer) {
            deviceCreateInfo.enabledLayerCount = static_cast<u32>(layerNames.size());
            deviceCreateInfo.ppEnabledLayerNames = layerNames.data();
        } else 
            deviceCreateInfo.enabledLayerCount = 0;

        VK_CHECK_RESULT(vkCreateDevice(this->physicalDevice, &deviceCreateInfo, nullptr, &(this->logicalDevice)));

        vkGetDeviceQueue(logicalDevice, queueFamilyInices.graphics, 0, &graphic);
        vkGetDeviceQueue(logicalDevice, queueFamilyInices.compute, 0, &compute);
        vkGetDeviceQueue(logicalDevice, queueFamilyInices.present, 0, &present);
    }

    void VulkanDevice::destroyLogicalDevice()
    {
        if (logicalDevice)
            vkDestroyDevice(logicalDevice, nullptr);
    }
}


/// Very old code 
// VulkanDevice::VulkanDevice(VkPhysicalDevice physicalDev)
// {
//     assert(physicalDev);
//     this->physicalDevice = physicalDev;

//     vkGetPhysicalDeviceProperties(physicalDevice, &properties);
//     vkGetPhysicalDeviceFeatures(physicalDevice, &features);
//     vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

//     u32 queueFamilyCount = 0;
//     vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

//     assert(queueFamilyCount > 0);
//     queueFamilyperporties.resize(queueFamilyCount);
//     vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilyperporties.data());
    
//     u32 extCount = 0;
//     vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, nullptr);
//     if (extCount > 0){
//         std::vector<VkExtensionProperties> extensions(extCount);
//         if (vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, &extensions.front()) == VK_SUCCESS){
//             for (auto ext: extensions)
//             supportedExtensions.push_back(ext.extensionName);
//         }
//     }
// }

// VulkanDevice::~VulkanDevice()
// {
//     if (commandPool)
//         vkDestroyCommandPool(logicalDevice, commandPool, nullptr);
//     if (logicalDevice)
//         vkDestroyDevice(logicalDevice, nullptr);
// }

// u32 VulkanDevice::getQueueFamilyIndex(VkQueueFlagBits queueFlags) const 
// {
//     if (queueFlags & VK_QUEUE_COMPUTE_BIT){
//         for (u32 i = 0; i < static_cast<u32>(queueFamilyperporties.size()); i++){
//             if ((queueFamilyperporties[i].queueFlags & queueFlags) && ((queueFamilyperporties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0))
//                 return i;
//         }
//     }
//     if (queueFlags & VK_QUEUE_TRANSFER_BIT){
//         for (u32 i = 0; i < static_cast<u32>(queueFamilyperporties.size()); i++){
//             if ((queueFamilyperporties[i].queueFlags & queueFlags) && 
//             ((queueFamilyperporties[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) == 0) && 
//             ((queueFamilyperporties[i].queueFlags & VK_QUEUE_COMPUTE_BIT) == 0))
//                 return i;
//         }
//     }
//     for (u32 i = 0; i < static_cast<u32>(queueFamilyperporties.size()); i++){
//         if (queueFamilyperporties[i].queueFlags & queueFlags)
//             return i;
//     }
//     throw std::runtime_error("Could not find matching queue family index.");
// }

// bool VulkanDevice::extensionSupported(std::string extension)
// {
//     return (std::find(supportedExtensions.begin(), supportedExtensions.end(), extension) != supportedExtensions.end());
// }

// void VulkanDevice::createCommandPool(u32 queueFamilyIndex, VkCommandPoolCreateFlags createFlags)
// {
//     VkCommandPoolCreateInfo cmdPoolInfo{};
//     cmdPoolInfo.sType               = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
//     cmdPoolInfo.queueFamilyIndex    = queueFamilyIndex;
//     cmdPoolInfo.flags               = createFlags;
//     VK_CHECK_RESULT(vkCreateCommandPool(this->logicalDevice, &cmdPoolInfo, nullptr, &(this->commandPool)));
// }

// VkResult VulkanDevice::createLogicalDevice(VkPhysicalDeviceFeatures enabledFeatures, std::vector<const char*> enabledExtensions, void *nextChain, bool useSwapChain, VkQueueFlags requestedQueueTypes)
// {
//     std::vector<VkDeviceQueueCreateInfo> queueCreateInfos{};
//     const float defaultQueuePriority = 0.0f;

//     // Graphics queue
//     if (requestedQueueTypes & VK_QUEUE_GRAPHICS_BIT){
//         queueFamilyInices.graphics = getQueueFamilyIndex(VK_QUEUE_GRAPHICS_BIT);
//         VkDeviceQueueCreateInfo queueInfo{};

//         queueInfo.sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//         queueInfo.queueFamilyIndex  = queueFamilyInices.graphics;
//         queueInfo.queueCount        = 1;
//         queueInfo.pQueuePriorities  = &defaultQueuePriority;
//         queueCreateInfos.push_back(queueInfo);
//     } else 
//         queueFamilyInices.graphics = VK_NULL_HANDLE;
//     // Compute queue
//     if (requestedQueueTypes & VK_QUEUE_COMPUTE_BIT){
//         queueFamilyInices.compute = getQueueFamilyIndex(VK_QUEUE_COMPUTE_BIT);
//         if (queueFamilyInices.compute != queueFamilyInices.graphics){
//             VkDeviceQueueCreateInfo queueInfo{};

//             queueInfo.sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//             queueInfo.queueFamilyIndex  = queueFamilyInices.compute;
//             queueInfo.queueCount        = 1;
//             queueInfo.pQueuePriorities  = &defaultQueuePriority;
//             queueCreateInfos.push_back(queueInfo);   
//         }
//     } else 
//         queueFamilyInices.compute = queueFamilyInices.graphics;
//     // Dedicated transfer queue
//     if (requestedQueueTypes & VK_QUEUE_TRANSFER_BIT){
//         queueFamilyInices.transfer = getQueueFamilyIndex(VK_QUEUE_TRANSFER_BIT);
//         if ((queueFamilyInices.transfer != queueFamilyInices.graphics) && (queueFamilyInices.transfer != queueFamilyInices.compute)){
//             VkDeviceQueueCreateInfo queueInfo{};

//             queueInfo.sType             = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
//             queueInfo.queueFamilyIndex  = queueFamilyInices.transfer;
//             queueInfo.queueCount        = 1;
//             queueInfo.pQueuePriorities  = &defaultQueuePriority;
//             queueCreateInfos.push_back(queueInfo);
//         }
//     } else 
//         queueFamilyInices.transfer = queueFamilyInices.graphics;

//     // logical device representation
//     std::vector<const char *> deviceExtensions(enabledExtensions);
//     if (useSwapChain) 
//         deviceExtensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
//     VkDeviceCreateInfo deviceCreateInfo = {};
//     deviceCreateInfo.sType              = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
//     deviceCreateInfo.queueCreateInfoCount   = static_cast<u32>(queueCreateInfos.size());
//     deviceCreateInfo.pQueueCreateInfos  = queueCreateInfos.data();
//     deviceCreateInfo.pEnabledFeatures   = &enabledFeatures;

//     // Next Chain support
//     VkPhysicalDeviceFeatures2 physicalDeviceFeatures2{};
//     if (nextChain) {
//         physicalDeviceFeatures2.sType   = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
//         physicalDeviceFeatures2.features = enabledFeatures;
//         physicalDeviceFeatures2.pNext   = nextChain;
        
//         deviceCreateInfo.pEnabledFeatures = nullptr;
//         deviceCreateInfo.pNext          = &physicalDeviceFeatures2;
//     }

//     if (extensionSupported(VK_EXT_DEBUG_MARKER_EXTENSION_NAME)) {
//         deviceExtensions.push_back(VK_EXT_DEBUG_MARKER_EXTENSION_NAME);
//         enableDebugMarkers = true;
//     }

//     if (deviceExtensions.size() > 0) {
//         for (const char * required: deviceExtensions){
//             if (!extensionSupported(required)) {
//                 std::cerr << "Required extension \"" << required << "\" by device." << std::endl;
//             }
//         }
//         deviceCreateInfo.enabledExtensionCount = static_cast<u32>(deviceExtensions.size());
//         deviceCreateInfo.ppEnabledExtensionNames = deviceExtensions.data();
//     }

//     this->enabledFeatures = enabledFeatures;

//     VkResult res = vkCreateDevice(this->physicalDevice, &deviceCreateInfo, nullptr, &(this->logicalDevice));
//     if (res != VK_SUCCESS)
//         return res;
    
//     createCommandPool(queueFamilyInices.graphics);

//     return res;
// }
