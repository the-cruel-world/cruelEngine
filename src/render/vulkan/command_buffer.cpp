#include "command_buffer.hpp"

#include "command_pool.hpp"
#include "logical_device.hpp"
#include "queue.hpp"

namespace cruelEngine::cruelRender
{
CommandBuffer::CommandBuffer(const CommandPool &_commandPool, VkCommandBufferLevel _level) :
    commandPool(_commandPool), level(_level)
{
    VkCommandBufferAllocateInfo allocate_info{VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};

    allocate_info.commandPool        = commandPool.GetHandle();
    allocate_info.commandBufferCount = 1;
    allocate_info.level              = level;

    VK_CHECK_RESULT(
        vkAllocateCommandBuffers(commandPool.GetDevice().get_handle(), &allocate_info, &handle));

    cmdState = CmdStateFlags::CMD_STATE_FREE;
}

CommandBuffer::~CommandBuffer()
{
    if (handle != VK_NULL_HANDLE)
    {
        vkFreeCommandBuffers(commandPool.GetDevice().get_handle(), commandPool.GetHandle(), 1,
                             &handle);
    }
}

void CommandBuffer::begin()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags            = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
    beginInfo.pInheritanceInfo = nullptr; // Optional
    if (cmdState == CmdStateFlags::CMD_STATE_RECORDING)
        throw std::runtime_error("CommandBuffer::beginOneTime : can't start a new recording when "
                                 "commandbuffer is in another record.");
    VK_CHECK_RESULT(vkBeginCommandBuffer(handle, &beginInfo));
    cmdState = CmdStateFlags::CMD_STATE_RECORDING;
}

void CommandBuffer::end()
{
    VK_CHECK_RESULT(vkEndCommandBuffer(handle));
    cmdState = CmdStateFlags::CMD_STATE_IDLE;
}

void CommandBuffer::beginOneTime()
{
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    if (cmdState == CmdStateFlags::CMD_STATE_RECORDING)
        throw std::runtime_error("CommandBuffer::beginOneTime : can't start a new recording when "
                                 "commandbuffer is in another record.");
    VK_CHECK_RESULT(vkBeginCommandBuffer(handle, &beginInfo));
    cmdState = CmdStateFlags::CMD_STATE_RECORDING;
}

void CommandBuffer::endOneTime()
{
    vkEndCommandBuffer(handle);
    cmdState = CmdStateFlags::CMD_STATE_IDLE;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    VkFence fence{};
    vkCreateFence(commandPool.GetDevice().get_handle(), &fenceInfo, nullptr, &fence);
    vkResetFences(commandPool.GetDevice().get_handle(), 1, &fence);

    VkSubmitInfo submitInfo{};
    submitInfo.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &handle;
    Queue &queue = commandPool.GetDevice().get_queue_by_flags(VK_QUEUE_TRANSFER_BIT, 0);

    // wait for the queue to finish this command call using a fence instead wait for idle.
    vkQueueSubmit(queue.get_handle(), 1, &submitInfo, fence);
    vkWaitForFences(commandPool.GetDevice().get_handle(), 1, &fence, VK_TRUE, UINT64_MAX);
    vkDestroyFence(commandPool.GetDevice().get_handle(), fence, nullptr);
}

void CommandBuffer::begin_renderpass(const VkRenderPass & renderPass,
                                     const VkFramebuffer &frameBuffer,
                                     const VkExtent2D     extent2dMode)
{
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType             = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass        = renderPass;
    renderPassInfo.framebuffer       = frameBuffer;
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = extent2dMode;
    VkClearValue clearColor          = {0.0f, 0.0f, 0.0f, 1.0f};

    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues    = &clearColor;

    vkCmdBeginRenderPass(handle, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandBuffer::end_renderpass()
{
    vkCmdEndRenderPass(handle);
}

void CommandBuffer::bindPipeLine(VkPipeline &pipeLine, VkPipelineBindPoint flag)
{
    vkCmdBindPipeline(handle, flag, pipeLine);
}

void CommandBuffer::setViewport(uint32_t first_viewport, const std::vector<VkViewport> &viewports)
{
    vkCmdSetViewport(handle, first_viewport, u32(viewports.size()), viewports.data());
}

void CommandBuffer::setScissor(uint32_t first_scissor, const std::vector<VkRect2D> &scissors)
{
    vkCmdSetScissor(handle, first_scissor, u32(scissors.size()), scissors.data());
}

VkResult CommandBuffer::reset(ResetModeFlags resetMode)
{
    VkResult result = VK_SUCCESS;

    assert(resetMode == commandPool.GetResetMode() &&
           "[CommandBuffer] : Reset : Fatal! the reset mode must match the reset "
           "mode of the command pool.");

    if (resetMode == ResetModeFlags::ResetIndividually)
    {
        result = vkResetCommandBuffer(handle, VK_COMMAND_BUFFER_RESET_RELEASE_RESOURCES_BIT);
    }
    cmdState = CmdStateFlags::CMD_STATE_RESETED;

    return result;
}

void CommandBuffer::Release()
{
    cmdState = CmdStateFlags::CMD_STATE_FREE;
}

void CommandBuffer::SetOccupied()
{
    cmdState = CmdStateFlags::CMD_STATE_IDLE;
}

CommandBuffer::CmdStateFlags CommandBuffer::GetCmdState()
{
    return cmdState;
}

} // namespace cruelEngine::cruelRender