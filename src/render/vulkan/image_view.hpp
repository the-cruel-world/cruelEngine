#pragma once
#include "../vkcommon.h"

namespace cruelEngine
{
namespace cruelRender
{
class LogicalDevice;
class Image;

class ImageView
{
public:
    ImageView(LogicalDevice &device, VkImage *image, VkFormat &format);

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

    const VkImage &get_image() const
    {
        return *image;
    }

    VkFormat get_format() const
    {
        return format;
    }

    void set_image(VkImage &image);

private:
    LogicalDevice &device;
    VkImage *      image;
    VkImageView    handle;

    VkFormat format;
};

} // namespace cruelRender
} // namespace cruelEngine