#include "image.hpp"

#include "logical_device.hpp"

namespace cruelEngine
{
namespace cruelRender
{
Image::Image(LogicalDevice &device, const VkExtent3D &extent, const VkFormat &format,
             const VkImageUsageFlags &image_usage, VkSampleCountFlagBits sample_count,
             uint32_t mip_levels, uint32_t array_layers, VkImageTiling tiling,
             VkImageCreateFlags flags, uint32_t num_queue_families,
             const uint32_t *queue_families) :
    device{device}
{}

Image::Image(LogicalDevice &device, const VkImage &handle, const VkExtent3D &extent,
             const VkFormat &format, const VkImageUsageFlags &imageUsage,
             const VkSampleCountFlags sampleCount) :
    device{device},
    handle{handle},
    type{findImageType(extent)},
    extent{extent},
    format{format},
    usage{imageUsage},
    sampleCount{sampleCount}
{
    subresource.mipLevel   = 1;
    subresource.arrayLayer = 1;
}

Image::~Image()
{
    if (handle != VK_NULL_HANDLE && imageMemory != VK_NULL_HANDLE)
    {
        vkDestroyImage(device.get_handle(), handle, nullptr);
        vkFreeMemory(device.get_handle(), imageMemory, nullptr);
    }
}

VkImageType Image::findImageType(VkExtent3D extent)
{
    VkImageType result{};
    uint32_t    dim_num{0};

    if (extent.width >= 1)
    {
        dim_num++;
    }

    if (extent.height >= 1)
    {
        dim_num++;
    }

    if (extent.depth > 1)
    {
        dim_num++;
    }

    switch (dim_num)
    {
        case 1:
            result = VK_IMAGE_TYPE_1D;
            break;
        case 2:
            result = VK_IMAGE_TYPE_2D;
            break;
        case 3:
            result = VK_IMAGE_TYPE_3D;
            break;
        default:
            throw std::runtime_error("No image type found.");
            break;
    }

    return result;
}
} // namespace cruelRender
} // namespace cruelEngine