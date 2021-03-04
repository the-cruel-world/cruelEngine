#include "vkDevice.hpp"

namespace cruelEngine {
namespace VulkanContext {
    VkDeviceQueueFamilyIndex::VkDeviceQueueFamilyIndex(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, const VkQueueFlags& flags)
    {
        findIndices(physicalDevice, surface, flags);
    }
    
    void VkDeviceQueueFamilyIndex::findIndices(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface, const VkQueueFlags& flags)
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

    VulkanDevice::VulkanDevice(bool _validataonLayer, 
        const std::vector<const char*> &_layerNames, 
        const VkInstance &_instance, const VkSurfaceKHR &_surface,
        const std::vector<const char*> &_requiredExtensions,
        const VkPhysicalDeviceFeatures &_requiredFeatures,
        int idx) : 
        VulkanDevice(_validataonLayer, _layerNames, _instance, _surface, _requiredExtensions, _requiredFeatures)
    {
        selectPhysicalDevice(idx);
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

    VulkanDevice::~VulkanDevice() {
        //std::cout << "clean up VulkanDevice" << std::endl;
        destroyLogicalDevice(); 
    }

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

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(device, &supportedFeatures);
        if (requiredFeatures.samplerAnisotropy && !supportedFeatures.samplerAnisotropy)
            return false;

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
        //std::cout << "device destroy" << std::endl;
        if (logicalDevice)
            vkDestroyDevice(logicalDevice, nullptr);
    }
}
}