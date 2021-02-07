#include "swapChain.hpp"

namespace realEngine {
    SwapChain::SwapChain(const Instance &_instance, const VulkanDevice &_device)
        : instance(_instance), device(_device), supportDetails(device.physicalDevice, instance.surface)
    {
        chooseSwapSurfaceFormat();
        chooseSwapPresentMode();
        chooseSwapExtentMode();
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