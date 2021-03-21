#include "command_pool.hpp"

#include "command_buffer.hpp"
#include "logical_device.hpp"

namespace cruelEngine::cruelRender {
CommandPool::CommandPool(LogicalDevice &device, u32 queueFamilyIndex,
                         CommandBuffer::ResetMode resetMode)
    : device{device}, queueFamilyIndex{queueFamilyIndex}, resetMode{resetMode} {

  VkCommandPoolCreateFlags flags;

  switch (resetMode) {
  case CommandBuffer::ResetMode::ResetIndividually:
  case CommandBuffer::ResetMode::ReAllocate:
    flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    break;

  case CommandBuffer::ResetMode::ResetPool:
  default:
    flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
    break;
  }

  VkCommandPoolCreateInfo poolInfo{};
  poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  poolInfo.queueFamilyIndex = queueFamilyIndex;
  poolInfo.flags = flags;

  if (vkCreateCommandPool(device.get_handle(), &poolInfo, nullptr, &handle) !=
      VK_SUCCESS) {
    throw std::runtime_error(
        "createCommandPool: failed to create command pool!");
  }
}

CommandPool::~CommandPool() {
  primary_command_buffers.clear();
  secondary_command_buffers.clear();

  if (handle != VK_NULL_HANDLE) {
    vkDestroyCommandPool(device.get_handle(), handle, nullptr);
  }
}

CommandBuffer &
CommandPool::RequestCommandBuffer(const VkCommandBufferLevel &_level) {

  fprintf(stdout, "[CommandPool] Pool %p: active primary command buffers: %d\n",
          this, active_primary_command_buffer_count);
  fprintf(stdout,
          "[CommandPool] Pool %p: active secondary command buffers: %d\n", this,
          active_secondary_command_buffer_count);

  if (_level == VK_COMMAND_BUFFER_LEVEL_PRIMARY) {
    if (active_primary_command_buffer_count < primary_command_buffers.size()) {
      return *primary_command_buffers.at(active_primary_command_buffer_count++);
    }
    primary_command_buffers.push_back(std::make_unique<CommandBuffer>(
        *this, VK_COMMAND_BUFFER_LEVEL_PRIMARY));

    active_primary_command_buffer_count++;

    return *primary_command_buffers.back();
  } else if (_level == VK_COMMAND_BUFFER_LEVEL_SECONDARY) {
    if (active_secondary_command_buffer_count <
        secondary_command_buffers.size()) {
      return *secondary_command_buffers.at(
          active_secondary_command_buffer_count++);
    }
    secondary_command_buffers.push_back(std::make_unique<CommandBuffer>(
        *this, VK_COMMAND_BUFFER_LEVEL_SECONDARY));

    active_secondary_command_buffer_count++;

    return *secondary_command_buffers.back();
  } else {
    throw std::runtime_error("Wrong command level. primary or secondary.");
  }
}

VkResult CommandPool::ResetPool() {
  VkResult result = VK_SUCCESS;
  switch (resetMode) {
  case CommandBuffer::ResetMode::ResetIndividually: {
    result = ResetCommandBuffers();

    break;
  }
  case CommandBuffer::ResetMode::ResetPool: {
    result = vkResetCommandPool(device.get_handle(), handle, 0);

    if (result != VK_SUCCESS)
      return result;

    result = ResetCommandBuffers();

    break;
  }
  case CommandBuffer::ResetMode::ReAllocate: {
    primary_command_buffers.clear();
    active_primary_command_buffer_count = 0;

    secondary_command_buffers.clear();
    active_secondary_command_buffer_count = 0;

    break;
  }
  default:
    throw std::runtime_error("[CommandPool] : Reset : Unknown reset mode.");
  }
  return VK_SUCCESS;
}

VkResult CommandPool::ResetCommandBuffers() {
  VkResult result = VK_SUCCESS;

  for (auto &cmdBuffer : primary_command_buffers) {
    result = cmdBuffer->reset(resetMode);
    if (result != VK_SUCCESS)
      return result;
  }
  active_primary_command_buffer_count = 0;

  for (auto &cmdBuffer : secondary_command_buffers) {
    result = cmdBuffer->reset(resetMode);
    if (result != VK_SUCCESS)
      return result;
  }
  active_secondary_command_buffer_count = 0;

  return result;
}
} // namespace cruelEngine::cruelRender