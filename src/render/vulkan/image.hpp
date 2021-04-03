#pragma once
#include "../vkcommon.h"

namespace cruelEngine::cruelRender
{
class LogicalDevice;
class CommandBuffer;

void SetImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout,
                    VkImageLayout newLayout, CommandBuffer &singleTimeCmd);

class Image
{
public:
    enum class ImageType
    {
        ImageType_Swapchain = 1 << 0,
        ImageType_Others    = 1 << 1
    };
    Image(LogicalDevice &device, const VkExtent3D &extent, const VkFormat &format,
          VkImageUsageFlags image_usage, VkMemoryPropertyFlags memory_flags,
          VkSampleCountFlagBits sample_count = VK_SAMPLE_COUNT_1_BIT, uint32_t mip_levels = 1,
          uint32_t array_layers = 1, VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL,
          VkImageCreateFlags flags = 0, uint32_t num_queue_families = 0,
          const uint32_t *queue_families = nullptr);

    Image(LogicalDevice &device, const VkImage &handle, const VkExtent3D &extent,
          const VkFormat &format, VkImageUsageFlags imageUsage,
          VkSampleCountFlags sampleCount = VK_SAMPLE_COUNT_1_BIT);

    Image(const Image &) = delete;

    // Image(Image &&) = delete;

    Image operator=(const Image &) = delete;

    // Image operator=(Image &&) = delete;

    ~Image();

    LogicalDevice &    get_device() const;
    VkImage            get_handle() const;
    VkDeviceMemory     get_memory() const;
    VkDeviceSize       get_memory_size() const;
    VkImageSubresource get_subresource() const;
    const VkImageType &get_image_type() const;
    const VkFormat &   get_format() const;
    u32                getArrayLayers() const;
    void               copyData(void *data, VkDeviceSize size);

private:
    LogicalDevice &device;

    ImageType imageType = {ImageType::ImageType_Others};

    VkImage        handle      = VK_NULL_HANDLE;
    VkDeviceMemory imageMemory = VK_NULL_HANDLE;
    VkDeviceSize   imageMemorySize{};

    VkImageType        type{};
    VkExtent3D         extent{};
    VkFormat           format{};
    VkSampleCountFlags sampleCount{};
    VkImageTiling      tiling{};
    VkImageUsageFlags  usage{};
    VkImageSubresource subresource{};
    u32                arrayLayers;

    VkImageType findImageType(VkExtent3D extent);
};
} // namespace cruelEngine::cruelRender