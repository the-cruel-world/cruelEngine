#pragma once
#include "../vkcommon.h"
#include "command_buffer.hpp"

namespace cruelEngine {
namespace cruelRender {

class CommandBuffer;

class LogicalDevice;

/** \brief A wrap of vulkan command pool for easy management. */
class CommandPool {
public:
  /** Create a command pool, it requires a physical device. */
  CommandPool(LogicalDevice &_device, u32 queueFamilyIndex,
              CommandBuffer::ResetMode resetMode = CommandBuffer::ResetPool);

  virtual ~CommandPool();

  // void                    createCommandPool();

  /** Return the handle of the commandpool. */
  VkCommandPool GetHandle() const { return handle; }

  /** Return a reference to the vulkandevice instance. */
  LogicalDevice &GetDevice() const { return device; }

  /** \brief Request a new commandbuffer from the commandPool. */
  CommandBuffer &RequestCommandBuffer(const VkCommandBufferLevel &_level);

  VkResult ResetPool();

  u32 GetQueueFamilyIndex() const { return queueFamilyIndex; }

  CommandBuffer::ResetMode GetResetMode() const { return resetMode; }

private:
  LogicalDevice &device;

  VkCommandPool handle = VK_NULL_HANDLE;

  u32 queueFamilyIndex = 0;

  CommandBuffer::ResetMode resetMode = CommandBuffer::ResetPool;

  size_t thread_index = 0;

  std::vector<std::unique_ptr<CommandBuffer>> primary_command_buffers;

  u32 active_primary_command_buffer_count = 0;

  std::vector<std::unique_ptr<CommandBuffer>> secondary_command_buffers;

  u32 active_secondary_command_buffer_count = 0;

  VkResult ResetCommandBuffers();
};
} // namespace cruelRender
} // namespace cruelEngine