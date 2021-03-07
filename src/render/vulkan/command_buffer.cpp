#include "command_buffer.hpp"

#include "command_pool.hpp"
#include "logical_device.hpp"
#include "queue.hpp"

namespace cruelEngine {
namespace cruelRender {

CommandBuffer::CommandBuffer(const CommandPool &_commandPool,
                             VkCommandBufferLevel _level)
    : commandPool(_commandPool), level(_level) {
  VkCommandBufferAllocateInfo allocate_info{
      VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO};

  allocate_info.commandPool = commandPool.get_handle();
  allocate_info.commandBufferCount = 1;
  allocate_info.level = level;

  VK_CHECK_RESULT(vkAllocateCommandBuffers(
      commandPool.get_device().get_handle(), &allocate_info, &handle));
}

CommandBuffer::~CommandBuffer() {
  if (handle != VK_NULL_HANDLE) {
    vkFreeCommandBuffers(commandPool.get_device().get_handle(),
                         commandPool.get_handle(), 1, &handle);
  }
}

void CommandBuffer::begin() {
  isRecord = true;
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
  beginInfo.pInheritanceInfo = nullptr; // Optional

  VK_CHECK_RESULT(vkBeginCommandBuffer(handle, &beginInfo));
}

void CommandBuffer::end() {
  isRecord = false;
  VK_CHECK_RESULT(vkEndCommandBuffer(handle));
}

void CommandBuffer::beginOneTime() {
  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  VK_CHECK_RESULT(vkBeginCommandBuffer(handle, &beginInfo));
}

void CommandBuffer::endOneTime() {
  vkEndCommandBuffer(handle);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &handle;

  Queue &queue =
      commandPool.get_device().get_queue_by_flags(VK_QUEUE_TRANSFER_BIT, 0);

  vkQueueSubmit(queue.get_handle(), 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(queue.get_handle());
  vkFreeCommandBuffers(commandPool.get_device().get_handle(),
                       commandPool.get_handle(), 1, &handle);
  //! Vulkan doesn't reset handle to VK_NULL_HANDLE after freecommandbuffer.
  handle = VK_NULL_HANDLE;
}

void CommandBuffer::begin_renderpass(const VkRenderPass &renderPass,
                                     const VkFramebuffer &frameBuffer,
                                     const VkExtent2D extent2dMode) {
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

void CommandBuffer::end_renderpass() { vkCmdEndRenderPass(handle); }

void CommandBuffer::bindPipeLine(VkPipeline &pipeLine,
                                 VkPipelineBindPoint flag) {
  vkCmdBindPipeline(handle, flag, pipeLine);
}

void CommandBuffer::setViewport(uint32_t first_viewport,
                                const std::vector<VkViewport> &viewports) {
  vkCmdSetViewport(handle, first_viewport, u32(viewports.size()),
                   viewports.data());
}

void CommandBuffer::setScissor(uint32_t first_scissor,
                               const std::vector<VkRect2D> &scissors) {
  vkCmdSetScissor(handle, first_scissor, u32(scissors.size()), scissors.data());
}

} // namespace cruelRender
} // namespace cruelEngine