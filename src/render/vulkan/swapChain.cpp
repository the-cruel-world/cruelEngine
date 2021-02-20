#include "swapChain.hpp"

namespace cruelEngine {
namespace VulkanContext {
    SwapChain::SwapChain(const Instance &_instance, const VulkanDevice &_device)
        : instance(_instance), device(_device)
    {
        
    }

    void SwapChain::createSwapChain(u32 &_imageCount)
    {
        supportDetails.getDetails(device.physicalDevice, instance.surface);  

        chooseSwapSurfaceFormat();
        chooseSwapPresentMode();
        chooseSwapExtentMode();

        if (supportDetails.capabilities.maxImageCount > 0 && 
            _imageCount > supportDetails.capabilities.maxImageCount)
            _imageCount = supportDetails.capabilities.maxImageCount;
        else if (supportDetails.capabilities.minImageCount > 0 && 
            _imageCount < supportDetails.capabilities.minImageCount)
            _imageCount = supportDetails.capabilities.minImageCount;
        

        imageCount = _imageCount;

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = instance.surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent2dMode;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        const VkQueue graphicFamily = device.getPresentQueue();
        const VkQueue presentFamily = device.getPresentQueue();
        uint32_t queueFamilyIndices[] = {
            device.getGraphicsQueueIndex(),
            device.getPresentQueueIndex()
        };

        if (graphicFamily != presentFamily) {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        } else {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = supportDetails.capabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = presentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        VK_CHECK_RESULT(vkCreateSwapchainKHR(device.logicalDevice, &createInfo, nullptr, &handle));
    }

    void SwapChain::createImages()
    {
        vkGetSwapchainImagesKHR(device.logicalDevice, handle, &imageCount, nullptr);
        images.resize(imageCount);
        vkGetSwapchainImagesKHR(device.logicalDevice, handle, &imageCount, images.data());
    }

    void SwapChain::createImageViews()
    {
        imageViews.resize(images.size());
        for (size_t i = 0; i < images.size(); i++) 
        {
            VkImageViewCreateInfo createInfo{};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = images[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = surfaceFormat.format;

            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;

            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(device.logicalDevice, &createInfo, nullptr, &imageViews[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to create image views!");
            }
        }
    }
    void SwapChain::destroyImageViews()
    {
        for (auto imageView : imageViews) {
            vkDestroyImageView(device.logicalDevice, imageView, nullptr);
        }
    }

    SwapChain::~SwapChain(){
        //std::cout << "clean up SwapChain" << std::endl;
        destroyImageViews();
        if (handle != VK_NULL_HANDLE)
            vkDestroySwapchainKHR(device.logicalDevice, handle, nullptr);
    }

    void SwapChain::chooseSwapSurfaceFormat() {
        if (supportDetails.formats.size() == 1 && supportDetails.formats[0].format == VK_FORMAT_UNDEFINED) {
            surfaceFormat = {VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR};
        }
        for (const auto available : supportDetails.formats) {
            if (available.format == VK_FORMAT_B8G8R8A8_SRGB && available.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                surfaceFormat =  available;
            }
        }
        surfaceFormat = supportDetails.formats[0];
        colorSpace = surfaceFormat.colorSpace;
        colorFormat = surfaceFormat.format;
    }
    
    void SwapChain::chooseSwapPresentMode() {
        presentMode = VK_PRESENT_MODE_FIFO_KHR;

        for (const auto & avaiable : supportDetails.presentModes) {
            if (avaiable == VK_PRESENT_MODE_MAILBOX_KHR) {
                presentMode = avaiable;
            } else if (avaiable == VK_PRESENT_MODE_IMMEDIATE_KHR) {
                presentMode = avaiable;
            }
        }
    }

    void SwapChain::chooseSwapExtentMode() {
        if (supportDetails.capabilities.currentExtent.width != UINT32_MAX) {
            extent2dMode = supportDetails.capabilities.currentExtent;
        } else {
            int width, height;
            glfwGetFramebufferSize (instance.topWindow.window, &width, &height);

            VkExtent2D actualExtent = {
                static_cast<u32> (width),
                static_cast<u32> (height)
            };

            actualExtent.width = std::max(supportDetails.capabilities.minImageExtent.width, std::min(supportDetails.capabilities.maxImageExtent.width, actualExtent.width));
            actualExtent.height = std::max(supportDetails.capabilities.minImageExtent.height, std::max(supportDetails.capabilities.maxImageExtent.height, actualExtent.height));

            extent2dMode = actualExtent;
        }
    }
}
}