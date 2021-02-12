#include "buffer.hpp"

namespace realEngine {

    FrameBuffer::FrameBuffer(const VulkanDevice &_device, const SwapChain &_swapChain, const RenderPass &_renderPass)
        : device (_device), swapChain (_swapChain), renderPass (_renderPass)
    {}

    FrameBuffer::~FrameBuffer(){
        //std::cout << "clean up Frame Buffer" << std::endl;
        destroyFrameBuffer();
    }

    void FrameBuffer::createFrameBuffer()
    {
        //std::cout << "createFrameBuffer: Creating frame buffer" << std::endl;
        frameBuffers.resize(swapChain.imageViews.size());

        for (size_t i = 0; i < swapChain.imageViews.size(); i++) {
            VkImageView attachments[] = {
                swapChain.imageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass.renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChain.extent2dMode.width;
            framebufferInfo.height = swapChain.extent2dMode.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device.logicalDevice, &framebufferInfo, 
                        nullptr, &frameBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("createFrameBuffer: failed to create framebuffer!");
            }
        }
    }
    void FrameBuffer::destroyFrameBuffer()
    {
        //std::cout << "destroyFrameBuffer: Cleaning frame buffer" << std::endl;
        for (auto framebuffer : frameBuffers) {
            vkDestroyFramebuffer(device.logicalDevice, framebuffer, nullptr);
        }
    }

    CommandBuffer::~CommandBuffer() {
        //std::cout << "clean up command buffer" << std::endl;
        destroyCommandBuffer();
        destroyCommandPool();
    }

    void CommandBuffer::createCommandPool(){
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = device.getGraphicsQueueIndex();
        poolInfo.flags = 0; // Optional

        if (vkCreateCommandPool(device.logicalDevice, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            throw std::runtime_error("createCommandPool: failed to create command pool!");
        }
    }

    void CommandBuffer::destroyCommandPool() {
        vkDestroyCommandPool(device.logicalDevice, commandPool, nullptr);
    }

    void CommandBuffer::createCommandBuffer() {
        commandBuffers.resize(frameBuffers.frameBuffers.size());
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

        if (vkAllocateCommandBuffers(device.logicalDevice, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
            throw std::runtime_error("createCommandBuffer: failed to allocate command buffers!");
        }

        for (size_t i = 0; i < commandBuffers.size(); i++) {
            VkCommandBufferBeginInfo beginInfo{};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
            beginInfo.pInheritanceInfo = nullptr; // Optional

            if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
                throw std::runtime_error("createCommandBuffer: failed to begin recording command buffer!");
            }
            VkRenderPassBeginInfo renderPassInfo{};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = renderPass.renderPass;
            renderPassInfo.framebuffer = frameBuffers.frameBuffers[i];
            renderPassInfo.renderArea.offset = {0, 0};
            renderPassInfo.renderArea.extent = swapChain.extent2dMode;
            VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;

            vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

            vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeLine.pipeLine);

            vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

            vkCmdEndRenderPass(commandBuffers[i]);

            if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("failed to record command buffer!");
            }
        }
    }

    void CommandBuffer::destroyCommandBuffer() {
        //commandBuffers.clear();
    }
}