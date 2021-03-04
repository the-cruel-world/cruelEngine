#pragma once
#include "vkcommon.h"

namespace cruelEngine {
namespace VulkanContext {
    class LogicalDevice;

    class Image {
    public:
        Image(LogicalDevice &device);

        Image(const Image &) = delete;

        Image(Image &&) = delete;

        Image operator=(const Image &) = delete;

        ~Image();

        LogicalDevice   &get_device() const {return device;}

        VkImage         get_handle() const {return handle;}

        const VkImageType   &get_image_type() const {return type;}

        const VkFormat      &get_format() const {return format;}
    
    private:
        LogicalDevice       &device;

        VkImage             handle = VK_NULL_HANDLE;

        VkImageType         type {};

        VkFormat            format {};

        VkImageUsageFlags   usage {};
    };
}
}