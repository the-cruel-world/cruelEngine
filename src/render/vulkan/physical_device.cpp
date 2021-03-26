#include "physical_device.hpp"

#include "instance.hpp"

namespace cruelEngine
{
namespace cruelRender
{
bool isPhysicalDeviceSuitable(const VkPhysicalDevice &        physicalDevice,
                              const VkPhysicalDeviceFeatures &requiredFeatures,
                              const std::vector<const char *> requiredExtensions,
                              const VkQueueFlags &            flags)
{
    //! \brief Check whether the physical device support the required extensions
    bool                     extensionSupported  = true;
    std::vector<std::string> supportedExtensions = {};
    u32                      extCount            = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount, nullptr);
    if (extCount > 0)
    {
        std::vector<VkExtensionProperties> extensions(extCount);
        if (vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extCount,
                                                 &extensions.front()) == VK_SUCCESS)
        {
            for (auto ext : extensions)
            {
                supportedExtensions.push_back(ext.extensionName);
                // std::cout << ext.extensionName << std::endl;
            }
        }
    }
    for (const auto &requiredExtension : requiredExtensions)
    {
        // std::cout << "Required: " << requiredExtension << std::endl;
        if (find(supportedExtensions.begin(), supportedExtensions.end(), requiredExtension) ==
            supportedExtensions.end())
        {
            extensionSupported = false;
            break;
        }
    }
    if (extensionSupported == false)
    {
        std::cerr << "at least one extension is not supported by device" << std::endl;
        return false;
    }

    //! \brief Check whether the physical device support the queue fethures
    //! required or not.
    u32 queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount,
                                             queueFamilies.data());

    bool queueSupported;
    if (flags & VK_QUEUE_GRAPHICS_BIT)
    {
        queueSupported = false;
        for (auto queueFamily : queueFamilies)
        {
            if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT))
            {
                queueSupported = true;
                break;
            }
        }
        if (queueSupported == false)
        {
            std::cerr << "transfer queue is not supported by device" << std::endl;
            return false;
        }
    }

    if (flags & VK_QUEUE_COMPUTE_BIT)
    {
        queueSupported = false;
        for (auto queueFamily : queueFamilies)
        {
            if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT))
            {
                queueSupported = true;
                break;
            }
        }
        if (queueSupported == false)
        {
            std::cerr << "transfer queue is not supported by device" << std::endl;
            return false;
        }
    }

    if (flags & VK_QUEUE_TRANSFER_BIT)
    {
        queueSupported = false;
        for (auto queueFamily : queueFamilies)
        {
            if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT))
            {
                queueSupported = true;
                break;
            }
        }
        if (queueSupported == false)
        {
            std::cerr << "transfer queue is not supported by device" << std::endl;
            return false;
        }
    }

    // Todo The physical device feature support details is not ok. rewrite later
    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevice, &supportedFeatures);
    if (requiredFeatures.samplerAnisotropy && !supportedFeatures.samplerAnisotropy)
        return false;

    return true;
}

PhysicalDevice::PhysicalDevice(const VkPhysicalDevice &  _physicalDevice,
                               VkPhysicalDeviceFeatures &_requiredFeatures) :
    handle{_physicalDevice}, requiredFeatures(_requiredFeatures)
{
    vkGetPhysicalDeviceFeatures(handle, &features);
    vkGetPhysicalDeviceMemoryProperties(handle, &memoryProperties);
    vkGetPhysicalDeviceProperties(handle, &properties);
    // std::cout << "apiVersion: " << properties.apiVersion << std::endl;
    // std::cout << "driverVersion: " << properties.driverVersion << std::endl;
}

PhysicalDevice::~PhysicalDevice()
{
    std::cout << "gpu destroied" << std::endl;
}

const u32 PhysicalDevice::getMemoryTypeIndex(const u32 &                  filter,
                                             const VkMemoryPropertyFlags &flags) const
{
    for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++)
    {
        if ((filter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & flags) == flags)
        {
            return i;
        }
    }
    throw std::runtime_error("failed to find suitable memory type!");
}

DriverVersion PhysicalDevice::get_driver_version() const
{
    DriverVersion version;
    switch (properties.vendorID)
    {
        case 0x10DE: {
            version.major = (properties.driverVersion >> 22) & 0x3ff;
            version.minor = (properties.driverVersion >> 14) & 0x0ff;
            version.patch = (properties.driverVersion >> 6) & 0x0ff;
            break;
        }
        default: {
            version.major = VK_VERSION_MAJOR(properties.driverVersion);
            version.minor = VK_VERSION_MINOR(properties.driverVersion);
            ;
            version.patch = VK_VERSION_PATCH(properties.driverVersion);
            ;
            break;
        }
    }
    return version;
}

} // namespace cruelRender
} // namespace cruelEngine