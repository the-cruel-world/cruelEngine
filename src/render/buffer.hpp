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
        std::vector<VkFramebuffer>  frameBuffers={};
    
    private:
        const VulkanDevice          &device;

        const SwapChain             &swapChain;

        const RenderPass            &renderPass;
    };
    class CommandBuffer {
    public:
        CommandBuffer(const VulkanDevice &_device, const SwapChain &_swapChain,
            const RenderPass &_renderPass,  
            const PipeLine &_pipeLine, const FrameBuffer &_frameBuffer)
            : device(_device), swapChain(_swapChain), renderPass(_renderPass),
            pipeLine(_pipeLine), frameBuffers(_frameBuffer)
        {

        }
        virtual ~CommandBuffer();
        
        void                        createCommandBuffer();
        void                        destroyCommandBuffer();
        void                        createCommandPool();
        void                        destroyCommandPool();
    public: 
        std::vector<VkCommandBuffer> commandBuffers = {};
        VkCommandPool commandPool = VK_NULL_HANDLE;

    private:
        const VulkanDevice          &device;
        const SwapChain             &swapChain;
        const RenderPass            &renderPass;
        const PipeLine              &pipeLine;
        const FrameBuffer           &frameBuffers;
    };
}