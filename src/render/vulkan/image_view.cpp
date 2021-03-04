#include "image_view.hpp"
#include "logical_device.hpp"
#include "image.hpp"

namespace cruelEngine {
namespace VulkanContext {

    ImageView::ImageView(LogicalDevice &device, VkImage *image, VkFormat &format) : 
        device{device}, image{image}, format{format}
    {
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = *image;
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = format;

        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;

        VK_CHECK_RESULT (vkCreateImageView(device.get_handle(), &createInfo, nullptr, &handle));
    }

    ImageView::ImageView(ImageView &&other) :
        device{other.device}, image{other.image}, handle{other.handle}
    {
        other.handle = VK_NULL_HANDLE;
        other.image = nullptr;
    }

    ImageView::~ImageView ()
    {
        if (handle != VK_NULL_HANDLE)
            vkDestroyImageView(device.get_handle(), handle, nullptr);
    }

    void ImageView::set_image(VkImage &new_image)
    {
        image = &new_image;
    }
}
}