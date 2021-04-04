#pragma once
#include "../vkcommon.h"

namespace cruelEngine::cruelRender
{
class Instance;
class LogicalDevice;
class Image;
class ImageView;

struct SwapchainProperties
{
    VkSwapchainKHR old_swapchain;

    uint32_t image_count{3};

    uint32_t array_layers{1};

    VkImageUsageFlags image_usage{};

    VkCompositeAlphaFlagBitsKHR composite_alpha{};

    VkExtent2D extent{};

    VkSurfaceFormatKHR surface_format{};

    VkSurfaceTransformFlagBitsKHR transform{};

    VkPresentModeKHR present_mode;
};

class Swapchain
{
public:
    Swapchain(LogicalDevice &_device, VkSurfaceKHR &_surface, const VkExtent2D _extent,
              const u32 _imageCount, const VkSurfaceTransformFlagBitsKHR transform,
              const VkPresentModeKHR                   _presentMode,
              const std::vector<VkImageUsageFlagBits> &image_usage = {
                  VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_IMAGE_USAGE_TRANSFER_SRC_BIT});

    Swapchain(Swapchain &old_swapchain, LogicalDevice &_device, VkSurfaceKHR &_surface,
              const VkExtent2D _extent, const u32 _imageCount,
              const VkSurfaceTransformFlagBitsKHR transform, const VkPresentModeKHR _presentMode,
              const std::vector<VkImageUsageFlagBits> &image_usage = {
                  VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_IMAGE_USAGE_TRANSFER_SRC_BIT});

    Swapchain(Swapchain &old_swapchain, const VkExtent2D _extent);

    Swapchain(Swapchain &old_swapchain, const u32 _imageCount);

    Swapchain(Swapchain &&other);

    Swapchain(const Swapchain &) = delete;

    Swapchain &operator=(const Swapchain &) = delete;

    Swapchain &operator=(Swapchain &&) = delete;

    ~Swapchain();

public:
    bool is_valid() const
    {
        return false;
    }

    VkResult acquire_next_image(uint32_t &image_index, VkSemaphore image_acquired_semaphore,
                                VkFence fence = VK_NULL_HANDLE);

    const VkSwapchainKHR &get_handle() const
    {
        return handle;
    }

    const LogicalDevice &get_device() const
    {
        return device;
    }

    const SwapchainProperties &get_properties() const
    {
        return properties;
    }

    const VkExtent2D &get_extent() const
    {
        return properties.extent;
    }

    const VkSurfaceFormatKHR &get_surface_format() const
    {
        return properties.surface_format;
    }

    const VkPresentModeKHR &get_present_mode() const
    {
        return properties.present_mode;
    }

    const std::vector<VkImageView> &get_imageViews() const
    {
        return imageViews;
    }

    std::vector<VkImage> &get_images()
    {
        return images;
    }

    VkSurfaceKHR &get_surface()
    {
        return surface;
    };

private:
    LogicalDevice &device;

    VkSurfaceKHR &surface;

    SwapchainProperties properties;

    std::vector<VkImage> images;

    std::vector<VkImageView> imageViews;

    VkSwapchainKHR handle = VK_NULL_HANDLE;

    std::vector<VkPresentModeKHR> present_mode_priority_list = {VK_PRESENT_MODE_FIFO_KHR,
                                                                VK_PRESENT_MODE_MAILBOX_KHR};

    std::vector<VkSurfaceFormatKHR> surface_format_priority_list = {
        {VK_FORMAT_R8G8B8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR},
        {VK_FORMAT_B8G8R8A8_SRGB, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR},
        {VK_FORMAT_R8G8B8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR},
        {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR}};

    std::vector<VkImageUsageFlagBits> image_usage{};
};
} // namespace cruelEngine::cruelRender