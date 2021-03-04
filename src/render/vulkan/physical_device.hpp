#pragma once
#include "vkcommon.h"

namespace cruelEngine {
namespace VulkanContext {

    class Instance;

    bool isPhysicalDeviceSuitable (const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const VkPhysicalDeviceFeatures &requiredFeatures, const std::vector<const char*> requiredExtensions, const VkQueueFlags& flags);
    
    struct VkDeviceQueueFamilyIndex {
        i32 graphics = -1;
        i32 present = -1;
        i32 compute = -1;
        i32 transfer = -1;
        VkDeviceQueueFamilyIndex(const VkPhysicalDevice &physicalDevice, const VkSurfaceKHR &surface, const VkQueueFlags &flags);
    };

    struct DriverVersion
    {
        uint16_t major;
        uint16_t minor;
        uint16_t patch;
    };

    class PhysicalDevice {
    public:
        /*! \brief The Data Prototype of physical device (GPU)
            This will create a gpu object, which contains all the information related to this device.
            \param physicalDevice is the vulkan handle to a physical device 
            \param _requiredFeatures is the features that we want the physical device to have.
        */
        PhysicalDevice(const VkPhysicalDevice& physicalDevice, VkPhysicalDeviceFeatures &_requiredFeatures);

        ~PhysicalDevice();

        const VkPhysicalDevice&     get_handle() const {return handle;}
        
        /*! \brief Return the required memoty type index. 
            \param filter is the  asdfdsagdasg asgd
            \param flags is the aldhfgdla fhdasf 
        */
        const u32                   getMemoryTypeIndex(const u32& filter, const VkMemoryPropertyFlags& flags) const;

        const VkPhysicalDeviceProperties &get_properties() const {return properties;}

        /*! \brief Reture the information about the driver used by this gpu. */
        const DriverVersion         &get_driver_version() const;

        //! \brief Return all the supported features of this device. 
        const VkPhysicalDeviceFeatures& getDeviceFeatures() const {return features;}

        //! \brief Return the required features the must be supported by this device.
        const VkPhysicalDeviceFeatures& getRequiredFeatures() const {return requiredFeatures;}

        //! \brief Return the memory properties of this device.
        const VkPhysicalDeviceMemoryProperties& getMemoryProperties() const {return memoryProperties;}

        // const VkSampleCountFlagBits& getMultiSamplingLevel() const;

    private:

        VkPhysicalDevice            handle = VK_NULL_HANDLE;

        VkPhysicalDeviceFeatures    features{};

        VkPhysicalDeviceFeatures    &requiredFeatures;

        // The GPU properties
        VkPhysicalDeviceProperties  properties;

        // The GPU memory properties
        VkPhysicalDeviceMemoryProperties memoryProperties;

        // The GPU queue family properties
        // std::vector<VkQueueFamilyProperties> queueFamilyProperties {};

        // The depthFormat
        VkFormat                    depthFormat {};

        std::vector<const char *>   extension_features {};
    };
}
}