#pragma once
#include "../vkcommon.h"

namespace cruelEngine
{
namespace cruelRender
{
class LogicalDevice;
class Image;

bool is_depth_only_format(VkFormat format);

bool is_depth_stencil_format(VkFormat format);

class ImageView
{
public:
    ImageView(LogicalDevice &device, Image *image, VkFormat format = VK_FORMAT_UNDEFINED);

    ImageView(LogicalDevice &device, Image *image, VkImageViewType view_type,
              VkFormat format = VK_FORMAT_UNDEFINED, u32 base_mip_level = 0,
              u32 base_array_layer = 0, u32 mip_levels = 0, u32 array_layers = 0);

    ImageView(ImageView &) = delete;

    ImageView operator=(const ImageView &) = delete;

    ImageView(ImageView &&other);

    ~ImageView();

    VkImageView get_handle() const
    {
        return handle;
    }

    LogicalDevice &get_device() const
    {
        return device;
    }

    const Image &get_image() const
    {
        return *image;
    }

    VkFormat get_format() const
    {
        return format;
    }

    void set_image(Image &image);

private:
    LogicalDevice &device;
    Image *        image;
    VkImageView    handle;

    VkFormat format;

    VkImageSubresourceRange subresourceRange{};
};

} // namespace cruelRender
} // namespace cruelEngine