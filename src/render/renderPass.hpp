#pragma once
#include "../common.h"
#include "../types.h"

#include "vkDevice.hpp"
#include "swapChain.hpp"

#include "../debug/debug.hpp"

namespace realEngine {
    class RenderPass {
    public: 
        RenderPass(const Instance &_instance, const VulkanDevice &_device, const SwapChain &swapChain);
        virtual ~RenderPass();

    public:
        VkRenderPass            renderPass = VK_NULL_HANDLE;
        void                    createRenderPass();
        void                    destroyRenderPass();
    private:
        const SwapChain         &swapChain;
        const VulkanDevice      &device;
        const Instance          &instance;
    };
}