#pragma once
#include "../common.h"
#include "../types.h"
#include "vkDevice.hpp"
#include "instance.hpp"

namespace realEngine {

    struct swapChainSupportDetails  {
        VkSurfaceCapabilitiesKHR    capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;

        swapChainSupportDetails() { }
        swapChainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface){
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities);

            u32 formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
            if (formatCount!=0) {
                formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, formats.data());
            }

            u32 presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
            if (presentModeCount!=0) {
                presentModes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, presentModes.data());
            }
        }
    };
    class SwapChain {
    public:
        SwapChain(const Instance &_instance, const VulkanDevice &_device);
        virtual ~SwapChain();

    public: 
        std::vector<VkImage>    images;
        std::vector<VkImageView>imageViews;
        u32                     imageCount = 0;

        VkSwapchainKHR          swapChain = VK_NULL_HANDLE;
        VkColorSpaceKHR         coloeSpace;
        VkFormat                colorFormat;
        VkSurfaceFormatKHR      surfaceFormat;
        VkPresentModeKHR        presentMode;
        VkExtent2D              extent2dMode;

        swapChainSupportDetails supportDetails;
        virtual void            chooseSwapSurfaceFormat();
        virtual void            chooseSwapPresentMode();
        virtual void            chooseSwapExtentMode();
        
    private:
        const VulkanDevice      &device;
        const Instance          &instance;

    };
}