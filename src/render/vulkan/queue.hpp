#pragma once
#include "../vkcommon.h"

namespace cruelEngine {
namespace cruelRender {

class LogicalDevice;
class CommandBuffer;

class Queue {
public:
  Queue(LogicalDevice &_device, const u32 _family_index, const u32 _index,
        const VkQueueFamilyProperties _properties);

  Queue(const Queue &) = delete;

  Queue(Queue &&);

  Queue &operator=(const Queue &) = delete;

  Queue &operator=(Queue &&) = delete;

  virtual ~Queue(){};

  const VkQueue &get_handle() const { return handle; }

  const LogicalDevice &get_device() const { return device; }

  const u32 get_index() const { return index; }

  VkBool32 can_present(const VkSurfaceKHR &surface);

  const u32 get_family_index() const { return family_index; }

  const VkQueueFamilyProperties &get_properties() const { return properties; }

  VkResult submit(const std::vector<VkSubmitInfo> &submit_infos, VkFence fence);

  VkResult submit(const CommandBuffer &command_buffer, VkFence fence);

  VkResult present(const VkPresentInfoKHR &presentInfo);

  VkResult wait_idle();

private:
  LogicalDevice &device;

  VkQueue handle = VK_NULL_HANDLE;

  u32 family_index = 0;

  u32 index = 0;

  VkQueueFamilyProperties properties{};

  // bool            busy = false;
};
} // namespace cruelRender
} // namespace cruelEngine