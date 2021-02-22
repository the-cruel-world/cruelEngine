#pragma once
#include "vkcommon.h"

#include "vkDevice.hpp"
#include "instance.hpp"

namespace cruelEngine {
namespace VulkanContext {
    struct swapChainSupportDetails  {
        VkSurfaceCapabilitiesKHR    capabilities = {};
        std::vector<VkSurfaceFormatKHR> formats = {};
        std::vector<VkPresentModeKHR> presentModes = {};

        swapChainSupportDetails() { }
        swapChainSupportDetails(const VkPhysicalDevice &device, const VkSurfaceKHR &surface){
            getDetails(device, surface);
        }
        void cleanUp(){
            formats.clear();
            presentModes.clear();
        }
        void getDetails(const VkPhysicalDevice &device, const VkSurfaceKHR &surface){
            VK_CHECK_RESULT(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &capabilities));

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
    struct SwapChain {
    public:
        SwapChain(const Instance &_instance, const VulkanDevice &_device);

        virtual ~SwapChain();

        void update(const VkExtent2D& _extent2dMode);

        void update(const VkColorSpaceKHR& _colorSpace);

        void update(const VkFormat& _colorFormat);

    public:

        u32                     imageCount = 0;

        void                    createSwapChain(u32 &_imageCount);

        const VkSwapchainKHR&   get_handle() const {return handle;}

        const VulkanDevice&     get_device() const {return device;}

        const VkFormat&         get_colorFormat() const {return colorFormat;}

        const VkExtent2D&       get_extent() const {return extent2dMode;}

        const std::vector<VkImageView> & get_imageViews() const {return imageViews;}
        
    private:
        const Instance          &instance;
        const VulkanDevice      &device;

        VkColorSpaceKHR         colorSpace;
        VkFormat                colorFormat;
        VkSurfaceFormatKHR      surfaceFormat;
        VkPresentModeKHR        presentMode;
        VkExtent2D              extent2dMode;

        std::vector<VkImage>    images;
        std::vector<VkImageView>imageViews;

        VkSwapchainKHR          handle = VK_NULL_HANDLE;

        swapChainSupportDetails supportDetails;
        virtual void            chooseSwapSurfaceFormat();
        virtual void            chooseSwapPresentMode();
        virtual void            chooseSwapExtentMode();

        void                    createImages();

        void                    createImageViews();

        void                    destroyImageViews();

        void                    __create();

        void                    __destroy();

    };
}
}