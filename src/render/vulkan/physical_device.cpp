#include "physical_device.hpp"
#include "instance.hpp"

namespace cruelEngine {
namespace VulkanContext {

    VkDeviceQueueFamilyIndex::VkDeviceQueueFamilyIndex(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const VkQueueFlags &flags)
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

    bool isPhysicalDeviceSuitable (const VkPhysicalDevice &physicalDevice, 
                        const VkSurfaceKHR &surface, 
                        const VkPhysicalDeviceFeatures &requiredFeatures, 
                        const std::vector<const char*> requiredExtensions, 
                        const VkQueueFlags& flags)
    {
        // Extension support check
        bool extensionSupported = true;
        std::vector<std::string> supportedExtensions = {};
        u32 extCount = 0;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, nullptr);
        if (extCount > 0){
            std::vector<VkExtensionProperties> extensions(extCount);
            if (vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, &extensions.front()) == VK_SUCCESS){
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

        VkDeviceQueueFamilyIndex index(physicalDevice, surface, flags);

        //std::cout << "Flags: " << index.present << ",\t" << index.graphics << ",\t" << index.compute << std::endl;
        if (!(flags & VK_QUEUE_GRAPHICS_BIT) || index.graphics < 0 || index.present <0)
            queueSupported = false;
        if (!(flags & VK_QUEUE_COMPUTE_BIT) || index.compute < 0)
            queueSupported = false;
        if (!(flags & VK_QUEUE_TRANSFER_BIT) || index.transfer < 0)
           queueSupported = false;

        VkPhysicalDeviceFeatures supportedFeatures;
        vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);
        if (requiredFeatures.samplerAnisotropy && !supportedFeatures.samplerAnisotropy)
            return false;

        return queueSupported && extensionSupported;
    }

    PhysicalDevice::PhysicalDevice(const VkPhysicalDevice& _physicalDevice, VkPhysicalDeviceFeatures &_requiredFeatures) :
        handle {_physicalDevice}, requiredFeatures(_requiredFeatures)
    {
        vkGetPhysicalDeviceFeatures(handle, &features);
        vkGetPhysicalDeviceMemoryProperties(handle, &memoryProperties);
        vkGetPhysicalDeviceProperties(handle, &properties);
        // std::cout << "apiVersion: " << properties.apiVersion << std::endl;
        // std::cout << "driverVersion: " << properties.driverVersion << std::endl;
    }

    PhysicalDevice::~PhysicalDevice() {
        std::cout <<  "gpu destroied" << std::endl;
    }

    const u32 PhysicalDevice::getMemoryTypeIndex(const u32& filter, const VkMemoryPropertyFlags& flags) const 
    {
        for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
            if ((filter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & flags) == flags) {
                return i;
            }
        }
        throw std::runtime_error("failed to find suitable memory type!");
    }

    const DriverVersion& PhysicalDevice::get_driver_version() const
    {
        DriverVersion version;
        switch (properties.vendorID)
        {
            case 0x10DE:
            {
                version.major = (properties.driverVersion >> 22) & 0x3ff;
                version.minor = (properties.driverVersion >> 14) & 0x0ff;
                version.patch = (properties.driverVersion >> 6) & 0x0ff;
                break;
            }
            default:
            {
                version.major = VK_VERSION_MAJOR(properties.driverVersion);
                version.minor = VK_VERSION_MINOR(properties.driverVersion);;
                version.patch = VK_VERSION_PATCH(properties.driverVersion);;
                break;
            }
        }
        return version;
    }

}
}