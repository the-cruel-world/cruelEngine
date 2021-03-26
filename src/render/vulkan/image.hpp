#pragma once
#include "../vkcommon.h"

namespace cruelEngine
{
namespace cruelRender
{
class LogicalDevice;

class Image
{
public:
    Image(LogicalDevice &device, const VkExtent3D &extent, const VkFormat &format,
          const VkImageUsageFlags &image_usage,
          VkSampleCountFlagBits sample_count = VK_SAMPLE_COUNT_1_BIT, uint32_t mip_levels = 1,
          uint32_t array_layers = 1, VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL,
          VkImageCreateFlags flags = 0, uint32_t num_queue_families = 0,
          const uint32_t *queue_families = nullptr);

    Image(LogicalDevice &device, const VkImage &handle, const VkExtent3D &extent,
          const VkFormat &format, const VkImageUsageFlags &imageUsage,
          const VkSampleCountFlags sampleCount = VK_SAMPLE_COUNT_1_BIT);

    Image(const Image &) = delete;

    Image(Image &&) = delete;

    Image operator=(const Image &) = delete;

    Image operator=(Image &&) = delete;

    ~Image();

    LogicalDevice &get_device() const
    {
        return device;
    }

    VkImage get_handle() const
    {
        return handle;
    }

    const VkImageType &get_image_type() const
    {
        return type;
    }

    const VkFormat &get_format() const
    {
        return format;
    }

private:
    LogicalDevice &device;

    VkImage        handle      = VK_NULL_HANDLE;
    VkDeviceMemory imageMemory = VK_NULL_HANDLE;

    VkImageType        type{};
    VkExtent3D         extent{};
    VkFormat           format{};
    VkSampleCountFlags sampleCount{};
    VkImageTiling      tiling{};
    VkImageUsageFlags  usage{};
    VkImageSubresource subresource{};

    VkImageType findImageType(VkExtent3D extent);
};
} // namespace cruelRender
} // namespace cruelEngine