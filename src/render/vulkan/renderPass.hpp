#pragma once
#include "vkcommon.h"

#include "vkDevice.hpp"
#include "swapChain.hpp"

namespace cruelEngine {
namespace VulkanContext {
    struct RenderPass {
    public: 
        RenderPass(const VulkanDevice &_device, const SwapChain &swapChain);

        virtual ~RenderPass();

    public:

        void                    createRenderPass();

        const VkRenderPass&     get_handle() const {return handle;}

        const VulkanDevice&     get_device() const {return device;}

    private:

        const VulkanDevice      &device;

        const VkFormat          &colorFormat;

        VkRenderPass            handle = VK_NULL_HANDLE;
    };
}
}