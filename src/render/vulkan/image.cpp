#include "image.hpp"

#include "logical_device.hpp"
#include "physical_device.hpp"

namespace cruelEngine::cruelRender
{
Image::Image(LogicalDevice &device, const VkExtent3D &extent, const VkFormat &format,
             VkImageUsageFlags image_usage, VkMemoryPropertyFlags memory_flags,
             VkSampleCountFlagBits sample_count, uint32_t mip_levels, uint32_t array_layers,
             VkImageTiling tiling, VkImageCreateFlags flags, uint32_t num_queue_families,
             const uint32_t *queue_families) :
    device{device},
    type{findImageType(extent)},
    extent{extent},
    format{format},
    usage{image_usage},
    tiling{tiling}
{
    assert(mip_levels > 0 && "Image should have at least one level");
    assert(array_layers > 0 && "Image should have at least one layer");

    subresource.mipLevel   = mip_levels;
    subresource.arrayLayer = array_layers;
    subresource.aspectMask = 0;

    VkImageCreateInfo imageCI{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};

    imageCI.flags         = flags;
    imageCI.imageType     = type;
    imageCI.format        = format;
    imageCI.extent        = extent;
    imageCI.mipLevels     = mip_levels;
    imageCI.arrayLayers   = array_layers;
    imageCI.samples       = sample_count;
    imageCI.usage         = usage;
    imageCI.tiling        = tiling;
    imageCI.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;
    imageCI.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    if (num_queue_families > 0)
    {
        imageCI.sharingMode           = VK_SHARING_MODE_CONCURRENT;
        imageCI.queueFamilyIndexCount = num_queue_families;
        imageCI.pQueueFamilyIndices   = queue_families;
    }

    VK_CHECK_RESULT(vkCreateImage(device.get_handle(), &imageCI, nullptr, &handle));

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device.get_handle(), handle, &memRequirements);

    VkMemoryAllocateInfo allocInfo{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    allocInfo.allocationSize  = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(device.get_physicalDevice().get_handle(),
                                               memRequirements.memoryTypeBits, memory_flags);

    if (vkAllocateMemory(device.get_handle(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate image memory!");
    }
    vkBindImageMemory(device.get_handle(), handle, imageMemory, 0);
}

Image::Image(LogicalDevice &device, const VkImage &handle, const VkExtent3D &extent,
             const VkFormat &format, VkImageUsageFlags imageUsage, VkSampleCountFlags sampleCount) :
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

LogicalDevice &Image::get_device() const
{
    return device;
}

VkImage Image::get_handle() const
{
    return handle;
}

VkDeviceMemory Image::get_memory() const
{
    return imageMemory;
}

VkImageSubresource Image::get_subresource() const
{
    return subresource;
}

const VkImageType &Image::get_image_type() const
{
    return type;
}

const VkFormat &Image::Image::get_format() const
{
    return format;
}

u32 Image::getArrayLayers() const
{
    return arrayLayers;
}

} // namespace cruelEngine::cruelRender