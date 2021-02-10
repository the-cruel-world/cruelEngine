#pragma once

#include "../common.h"
#include "../types.h"

#include "../debug/debug.hpp"

#include "vkDevice.hpp"
#include "swapChain.hpp"
#include "pipeLine.hpp"
#include "renderPass.hpp"

namespace realEngine {
    class FrameBuffer {
    public:
        FrameBuffer(const VulkanDevice &_device, const SwapChain &_swapChain, const RenderPass &_renderPass);
        virtual ~FrameBuffer();

        void                        createFrameBuffer();
        void                        destroyFrameBuffer();
    public:
        std::vector<VkFramebuffer>  Framebuffers={};
    
    private:
        const VulkanDevice          &device;

        const SwapChain             &swapChain;

        const RenderPass            &renderPass;
    };
    class CommandBuffer {};
}