#include "swapchain.hpp"

#include "frame_buffer.hpp"
// #include "image.hpp"
// #include "image_view.hpp"
#include "instance.hpp"
#include "logical_device.hpp"
#include "physical_device.hpp"

namespace cruelEngine::cruelRender
{
u32 choose_image_count(u32 requestCount, u32 minCount, u32 maxCount)
{
    if (maxCount < minCount)
        return requestCount;
    requestCount = std::min(std::max(requestCount, minCount), maxCount);
    return requestCount;
}

VkExtent2D choose_extent(VkExtent2D requestExtent, VkExtent2D minExtent, VkExtent2D maxExtent)
{
    requestExtent.height =
        std::min(std::max(requestExtent.height, minExtent.height), maxExtent.height);
    requestExtent.width = std::min(std::max(requestExtent.width, minExtent.width), maxExtent.width);
    return requestExtent;
}

VkPresentModeKHR choose_present_mode(VkPresentModeKHR              request,
                                     std::vector<VkPresentModeKHR> available,
                                     std::vector<VkPresentModeKHR> priority)
{
    auto present_mode_it = std::find(available.begin(), available.end(), request);
    if (present_mode_it != available.end())
        return request;

    VkPresentModeKHR chosen_present_mode = VK_PRESENT_MODE_FIFO_KHR;
    for (auto &present_mode : priority)
    {
        present_mode_it = std::find(priority.begin(), priority.end(), present_mode);
        if (present_mode_it != priority.end())
            chosen_present_mode = present_mode;
    }
    return chosen_present_mode;
}

VkSurfaceFormatKHR choose_surface_format(VkSurfaceFormatKHR              request,
                                         std::vector<VkSurfaceFormatKHR> available,
                                         std::vector<VkSurfaceFormatKHR> priority)
{
    auto surface_format_it = std::find_if(
        available.begin(), available.end(), [&request](const VkSurfaceFormatKHR &format) {
            return (format.format == request.format && format.colorSpace == request.colorSpace);
        });
    if (surface_format_it != available.end())
        return request;

    for (auto &surface_format : priority)
    {
        surface_format_it =
            std::find_if(available.begin(), available.end(),
                         [&surface_format](const VkSurfaceFormatKHR &format) {
                             return (format.format == surface_format.format &&
                                     format.colorSpace == surface_format.colorSpace);
                         });
        if (surface_format_it != available.end())
            return surface_format;
    }
    return available[0];
}

VkSurfaceTransformFlagBitsKHR choose_transform(VkSurfaceTransformFlagBitsKHR request,
                                               VkSurfaceTransformFlagsKHR    supported,
                                               VkSurfaceTransformFlagBitsKHR current)
{
    return (request & supported) ? request : current;
}

VkCompositeAlphaFlagBitsKHR choose_composite_alpha(VkCompositeAlphaFlagBitsKHR request,
                                                   VkCompositeAlphaFlagsKHR    supported)
{
    if (request & supported)
    {
        return request;
    }

    static const std::vector<VkCompositeAlphaFlagBitsKHR> composite_alpha_flags = {
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR, VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR};

    for (VkCompositeAlphaFlagBitsKHR composite_alpha : composite_alpha_flags)
    {
        if (composite_alpha & supported)
            return composite_alpha;
    }
    throw std::runtime_error("No compatible composite alpha found.");
}

// std::vector<VkImageUsageFlagBits> choose_image_usage(const
// std::vector<VkImageUsageFlagBits> &requested,
//     VkImageUsageFlags supported_image_usage, VkFormatFeatureFlags
//     supported_features)
// {
//     std::vector<VkImageUsageFlagBits> validated_image_usage_flags;
//     for (auto flagBit : requested)
//     {
//         if (flagBit & )
//     }
// }

VkImageUsageFlags composite_image_flags(std::vector<VkImageUsageFlagBits> &image_usage_flags)
{
    VkImageUsageFlags image_usage{};
    for (auto flag : image_usage_flags)
    {
        image_usage |= flag;
    }
    return image_usage;
}

Swapchain::Swapchain(LogicalDevice &_device, VkSurfaceKHR &_surface, const VkExtent2D _extent,
                     const u32 _imageCount, const VkSurfaceTransformFlagBitsKHR _transform,
                     const VkPresentModeKHR                   _presentMode,
                     const std::vector<VkImageUsageFlagBits> &_image_usage) :
    Swapchain{*this, _device, _surface, _extent, _imageCount, _transform, _presentMode}
{}

Swapchain::Swapchain(Swapchain &old_swapchain, LogicalDevice &_device, VkSurfaceKHR &_surface,
                     const VkExtent2D _extent, const u32 _imageCount,
                     const VkSurfaceTransformFlagBitsKHR      _transform,
                     const VkPresentModeKHR                   _presentMode,
                     const std::vector<VkImageUsageFlagBits> &_image_usage) :
    device{_device}, surface{_surface}
{
    present_mode_priority_list   = old_swapchain.present_mode_priority_list;
    surface_format_priority_list = old_swapchain.surface_format_priority_list;

    VkSurfaceCapabilitiesKHR capabilities;
    VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
        device.get_physicalDevice().get_handle(), surface, &capabilities));

    u32                             formatCount;
    std::vector<VkSurfaceFormatKHR> formats;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device.get_physicalDevice().get_handle(), surface,
                                         &formatCount, nullptr);
    if (formatCount != 0)
    {
        formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device.get_physicalDevice().get_handle(), surface,
                                             &formatCount, formats.data());
    }

    u32                           presentModeCount;
    std::vector<VkPresentModeKHR> presentModes;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device.get_physicalDevice().get_handle(), surface,
                                              &presentModeCount, nullptr);
    if (presentModeCount != 0)
    {
        presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device.get_physicalDevice().get_handle(), surface,
                                                  &presentModeCount, presentModes.data());
    }

    properties.image_count =
        choose_image_count(_imageCount, capabilities.minImageCount, capabilities.maxImageCount);
    properties.extent =
        choose_extent(_extent, capabilities.minImageExtent, capabilities.maxImageExtent);
    properties.present_mode =
        choose_present_mode(_presentMode, presentModes, present_mode_priority_list);

    std::string present_mode;
    for (auto &a : presentModes)
    {
        present_mode += " " + std::to_string(a) + ",";
    }
    CRUEL_LOG("PresentMode supported: %s\n", present_mode.c_str());

    CRUEL_LOG("PresentMode Selected: %s\n",
              properties.present_mode == VK_PRESENT_MODE_FIFO_KHR ?
                  "VK_PRESENT_MODE_FIFO_KHR" :
                  (properties.present_mode == VK_PRESENT_MODE_MAILBOX_KHR ?
                       "VK_PRESENT_MODE_MAILBOX_KHR" :
                       (properties.present_mode == VK_PRESENT_MODE_IMMEDIATE_KHR ?
                            "VK_PRESENT_MODE_IMMEDIATE_KHR" :
                            "other")));

    properties.surface_format =
        choose_surface_format(properties.surface_format, formats, surface_format_priority_list);

    properties.transform       = choose_transform(_transform, capabilities.supportedTransforms,
                                            capabilities.currentTransform);
    properties.composite_alpha = choose_composite_alpha(VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
                                                        capabilities.supportedCompositeAlpha);
    image_usage                = _image_usage; //  choose_image_usage();
    properties.image_usage     = composite_image_flags(image_usage);
    // set array layers
    properties.array_layers  = 1U;
    properties.old_swapchain = old_swapchain.get_handle();

    // properties.present_mode   = choose_present_mode(properties.present_mode,
    // presentModes, present_mode_priority_list);
    VkSwapchainCreateInfoKHR create_info{VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR};
    create_info.minImageCount    = properties.image_count;
    create_info.imageExtent      = properties.extent;
    create_info.presentMode      = properties.present_mode;
    create_info.imageFormat      = properties.surface_format.format;
    create_info.imageColorSpace  = properties.surface_format.colorSpace;
    create_info.imageArrayLayers = properties.array_layers;
    create_info.imageUsage       = properties.image_usage;
    create_info.preTransform     = properties.transform;
    create_info.compositeAlpha   = properties.composite_alpha;
    create_info.oldSwapchain     = properties.old_swapchain;
    // create_info.queueFamilyIndexCount = 1;
    // u32 familyIndices[] = {device.get_suitable_present_queue(surface,
    // 0).get_family_index()}; create_info.pQueueFamilyIndices = familyIndices;
    create_info.surface = surface;

    VK_CHECK_RESULT(vkCreateSwapchainKHR(device.get_handle(), &create_info, nullptr, &handle));

    u32 available_images = 0;
    VK_CHECK_RESULT(
        vkGetSwapchainImagesKHR(device.get_handle(), handle, &available_images, nullptr));

    images.resize(available_images);
    VK_CHECK_RESULT(
        vkGetSwapchainImagesKHR(device.get_handle(), handle, &available_images, images.data()));

    CRUEL_LOG("%s\n", "SwapChain Created");
}

Swapchain::Swapchain(Swapchain &old_swapchain, const VkExtent2D _extent) :
    Swapchain(old_swapchain, old_swapchain.device, old_swapchain.surface, _extent,
              old_swapchain.properties.image_count, old_swapchain.properties.transform,
              old_swapchain.properties.present_mode)
{}

Swapchain::~Swapchain()
{
    if (handle != VK_NULL_HANDLE)
        vkDestroySwapchainKHR(device.get_handle(), handle, nullptr);
}

VkResult Swapchain::acquire_next_image(uint32_t &image_index, VkSemaphore image_acquired_semaphore,
                                       VkFence fence)
{
    return vkAcquireNextImageKHR(device.get_handle(), handle, UINT64_MAX, image_acquired_semaphore,
                                 fence, &image_index);
}
} // namespace cruelEngine::cruelRender