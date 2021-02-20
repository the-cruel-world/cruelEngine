#include "commandBuffer.hpp"

namespace cruelEngine {
namespace VulkanContext {
    // CommandBuffer::~CommandBuffer() {
    //     //std::cout << "clean up command buffer" << std::endl;
    //     destroyCommandBuffer();
    //     destroyCommandPool();
    // }

    // void CommandBuffer::createCommandPool(){
    //     VkCommandPoolCreateInfo poolInfo{};
    //     poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    //     poolInfo.queueFamilyIndex = device.getGraphicsQueueIndex();
    //     poolInfo.flags = 0; // Optional

    //     if (vkCreateCommandPool(device.logicalDevice, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
    //         throw std::runtime_error("createCommandPool: failed to create command pool!");
    //     }
    // }

    // void CommandBuffer::destroyCommandPool() {
    //     vkDestroyCommandPool(device.logicalDevice, commandPool, nullptr);
    // }

    // void CommandBuffer::createCommandBuffer() {
    //     commandBuffers.resize(frameBuffers.frameBuffers.size());
    //     VkCommandBufferAllocateInfo allocInfo{};
    //     allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    //     allocInfo.commandPool = commandPool;
    //     allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    //     allocInfo.commandBufferCount = (uint32_t) commandBuffers.size();

    //     if (vkAllocateCommandBuffers(device.logicalDevice, &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
    //         throw std::runtime_error("createCommandBuffer: failed to allocate command buffers!");
    //     }

    //     for (size_t i = 0; i < commandBuffers.size(); i++) {
    //         VkCommandBufferBeginInfo beginInfo{};
    //         beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    //         beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    //         beginInfo.pInheritanceInfo = nullptr; // Optional

    //         if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
    //             throw std::runtime_error("createCommandBuffer: failed to begin recording command buffer!");
    //         }
    //         VkRenderPassBeginInfo renderPassInfo{};
    //         renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    //         renderPassInfo.renderPass = renderPass.renderPass;
    //         renderPassInfo.framebuffer = frameBuffers.frameBuffers[i];
    //         renderPassInfo.renderArea.offset = {0, 0};
    //         renderPassInfo.renderArea.extent = swapChain.extent2dMode;
    //         VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
    //         renderPassInfo.clearValueCount = 1;
    //         renderPassInfo.pClearValues = &clearColor;

    //         vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

    //         vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeLine.pipeLine);

    //         vkCmdDraw(commandBuffers[i], 3, 1, 0, 0);

    //         vkCmdEndRenderPass(commandBuffers[i]);

    //         if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
    //             throw std::runtime_error("failed to record command buffer!");
    //         }
    //     }
    // }

    // void CommandBuffer::destroyCommandBuffer() {
    //     //commandBuffers.clear();
    // }

    CommandBuffer::CommandBuffer (const CommandPool &_commandPool, VkCommandBufferLevel _level) : 
        commandPool (_commandPool), 
        level(_level)
    {
        VkCommandBufferAllocateInfo allocate_info {VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};

        allocate_info.commandPool = commandPool.get_handle();
        allocate_info.commandBufferCount = 1;
        allocate_info.level = level;
        
        VK_CHECK_RESULT(vkAllocateCommandBuffers(commandPool.get_device().logicalDevice, &allocate_info, &handle));
    }

    CommandBuffer::~CommandBuffer()
    {
        if (handle != VK_NULL_HANDLE) 
        {
            vkFreeCommandBuffers(commandPool.get_device().logicalDevice, commandPool.get_handle(), 1, &handle);
        }
    }

    void CommandBuffer::begin() 
    {
        isRecord = true;
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
        beginInfo.pInheritanceInfo = nullptr; // Optional

        VK_CHECK_RESULT (vkBeginCommandBuffer(handle, &beginInfo));
    }

    void CommandBuffer::end()
    {
        isRecord = false;
        VK_CHECK_RESULT (vkEndCommandBuffer(handle));
    }

    void CommandBuffer::begin_renderpass(const VkRenderPass &renderPass, const VkFramebuffer &frameBuffer, const VkExtent2D extent2dMode)
    {
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass;
        renderPassInfo.framebuffer = frameBuffer;
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = extent2dMode;
        VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
        
        renderPassInfo.clearValueCount = 1;
        renderPassInfo.pClearValues = &clearColor;

        vkCmdBeginRenderPass(handle, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    }

    void CommandBuffer::end_renderpass()
    {
        vkCmdEndRenderPass(handle);
    }

    void CommandBuffer::bindPipeLine(VkPipeline& pipeLine, VkPipelineBindPoint flag) {
        vkCmdBindPipeline(handle, flag, pipeLine);
    }


}
}