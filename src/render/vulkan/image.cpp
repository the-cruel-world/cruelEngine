#include "image.hpp"

#include "command_buffer.hpp"
#include "logical_device.hpp"
#include "physical_device.hpp"

namespace cruelEngine::cruelRender
{
void SetImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout,
                    VkImageLayout newLayout, CommandBuffer &singleTimeCmd)
{
    // Image berrier
    VkImageMemoryBarrier barrier{};
    barrier.sType                           = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier.oldLayout                       = oldLayout;
    barrier.newLayout                       = newLayout;
    barrier.srcQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    barrier.dstQueueFamilyIndex             = VK_QUEUE_FAMILY_IGNORED;
    barrier.image                           = image;
    barrier.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier.subresourceRange.baseMipLevel   = 0;
    barrier.subresourceRange.levelCount     = 1;
    barrier.subresourceRange.baseArrayLayer = 0;
    barrier.subresourceRange.layerCount     = 1;

    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;

    if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
    {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        sourceStage           = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        destinationStage      = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL &&
             newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
    {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        sourceStage           = VK_PIPELINE_STAGE_TRANSFER_BIT;
        destinationStage      = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else
    {
        throw std::invalid_argument("unsupported layout transition!");
    }

    vkCmdPipelineBarrier(singleTimeCmd.get_handle(), sourceStage, destinationStage, 0, 0, nullptr,
                         0, nullptr, 1, &barrier);
}

/**
 * Image invokes an image, and allocate memory.
 */
Image::Image(LogicalDevice &device, const VkExtent3D &extent, const VkFormat &format,
             VkImageUsageFlags image_usage, VkMemoryPropertyFlags memory_flags,
             VkSampleCountFlagBits sample_count, uint32_t mip_levels, uint32_t array_layers,
             VkImageTiling tiling, VkImageCreateFlags flags, uint32_t num_queue_families,
             const uint32_t *queue_families) :
    device{device},
    type{findImageType(extent)},
    extent{extent},
    format{format},
    tiling{tiling},
    usage{image_usage}
{
    assert(mip_levels > 0 && "Image should have at least one level");
    assert(array_layers > 0 && "Image should have at least one layer");

    imageType = ImageType::ImageType_Others;

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
        if (num_queue_families > 1)
            imageCI.sharingMode = VK_SHARING_MODE_CONCURRENT;
        imageCI.queueFamilyIndexCount = num_queue_families;
        imageCI.pQueueFamilyIndices   = queue_families;
    }

    VK_CHECK_RESULT(vkCreateImage(device.get_handle(), &imageCI, nullptr, &handle));

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(device.get_handle(), handle, &memRequirements);

    VkMemoryAllocateInfo allocInfo{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    allocInfo.allocationSize  = memRequirements.size;
    imageMemorySize           = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(device.get_physicalDevice().get_handle(),
                                               memRequirements.memoryTypeBits, memory_flags);

    if (vkAllocateMemory(device.get_handle(), &allocInfo, nullptr, &imageMemory) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to allocate image memory!");
    }
    vkBindImageMemory(device.get_handle(), handle, imageMemory, 0);
    std::cout << "x---x Image created! id: " << get_handle() << std::endl;
}

Image::Image(LogicalDevice &device, const VkImage &handle, const VkExtent3D &extent,
             const VkFormat &format, VkImageUsageFlags imageUsage, VkSampleCountFlags sampleCount) :
    device{device},
    handle{handle},
    type{findImageType(extent)},
    extent{extent},
    format{format},
    sampleCount{sampleCount},
    usage{imageUsage}
{
    subresource.mipLevel   = 1;
    subresource.arrayLayer = 1;

    imageType = ImageType::ImageType_Swapchain;

    std::cout << "x---x Image created! id: " << get_handle() << std::endl;
}

Image::Image(Image &&other) :
    device{other.device},
    imageType{other.imageType},
    handle{other.handle},
    imageMemory{other.imageMemory},
    imageMemorySize{other.imageMemorySize},
    type{other.type},
    extent{other.extent},
    format{other.format},
    sampleCount{other.sampleCount},
    tiling{other.tiling},
    usage{other.usage},
    subresource{other.subresource},
    arrayLayers{other.arrayLayers}
{
    std::cout << "x---x Image moving! id: " << get_handle() << std::endl;
    other.handle      = VK_NULL_HANDLE;
    other.imageMemory = VK_NULL_HANDLE;
}

Image::~Image()
{
    std::cout << "x---x Image destroy! id: " << get_handle() << std::endl;

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

VkDeviceSize Image::get_memory_size() const
{
    if (imageMemory != VK_NULL_HANDLE)
        return imageMemorySize;
    return 0;
}

void Image::copyData(void *data, VkDeviceSize size)
{
    if (size > imageMemorySize)
        throw std::runtime_error("Image::copyData fatal: data size larger than allocated space.");

    // Copy data to image memory
}

} // namespace cruelEngine::cruelRender