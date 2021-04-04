#include "image_view.hpp"

#include "image.hpp"
#include "logical_device.hpp"

namespace cruelEngine
{
namespace cruelRender
{
bool is_depth_only_format(VkFormat format)
{
    return format == VK_FORMAT_D16_UNORM || format == VK_FORMAT_D32_SFLOAT;
}

bool is_depth_stencil_format(VkFormat format)
{
    return format == VK_FORMAT_D16_UNORM_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT ||
           format == VK_FORMAT_D32_SFLOAT_S8_UINT || is_depth_only_format(format);
}

ImageView::ImageView(LogicalDevice &device, Image *image, VkFormat format) :
    device{device}, image{image}, format{format}
{
    subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    subresourceRange.baseMipLevel   = 0;
    subresourceRange.levelCount     = 1;
    subresourceRange.baseArrayLayer = 0;
    subresourceRange.layerCount     = 1;

    VkImageViewCreateInfo createInfo{};
    createInfo.sType    = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image    = image->get_handle();
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format   = format;

    createInfo.components.r     = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g     = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b     = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a     = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange = subresourceRange;

    VK_CHECK_RESULT(vkCreateImageView(device.get_handle(), &createInfo, nullptr, &handle));
    std::cout << "x---x ImageView Created! id: " << get_handle() << std::endl;
}

ImageView::ImageView(LogicalDevice &device, Image *image, VkImageViewType view_type,
                     VkFormat format, u32 base_mip_level, u32 base_array_layer, u32 mip_levels,
                     u32 array_layers) :
    device{device}, image{image}, format{format}
{
    subresourceRange.baseArrayLayer = base_array_layer;
    subresourceRange.baseMipLevel   = base_mip_level;
    subresourceRange.layerCount     = array_layers;
    subresourceRange.levelCount     = mip_levels;
    if (is_depth_stencil_format(format))
    {
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    }
    else
    {
        subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    VkImageViewCreateInfo createInfo{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
    createInfo.image            = image->get_handle();
    createInfo.viewType         = view_type;
    createInfo.format           = format;
    createInfo.components.r     = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g     = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b     = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a     = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange = subresourceRange;

    VK_CHECK_RESULT(vkCreateImageView(device.get_handle(), &createInfo, nullptr, &handle));
    std::cout << "x---x ImageView Created! id: " << get_handle() << std::endl;
}

ImageView::ImageView(ImageView &&other) :
    device{other.device}, image{other.image}, handle{other.handle}
{
    other.handle = VK_NULL_HANDLE;
    other.image  = nullptr;
    std::cout << "x---x ImageView Moved! id: " << get_handle() << std::endl;
}

ImageView::~ImageView()
{
    std::cout << "x---x ImageView Destroyed! id: " << get_handle() << std::endl;

    if (handle != VK_NULL_HANDLE)
        vkDestroyImageView(device.get_handle(), handle, nullptr);
}

void ImageView::set_image(Image &new_image)
{
    image = &new_image;
}
} // namespace cruelRender
} // namespace cruelEngine