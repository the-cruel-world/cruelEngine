#pragma once
#include "../vkcommon.h"

namespace cruelEngine {
namespace cruelRender {
class RenderPass;

class LogicalDevice;

class FrameBuffer {
public:
  FrameBuffer(LogicalDevice &device, const VkImageView &imageView,
              const VkExtent2D &extent, const RenderPass &renderPass);

  FrameBuffer(const FrameBuffer &) = delete;

  FrameBuffer(FrameBuffer &&other);

  FrameBuffer &operator=(const FrameBuffer &) = delete;

  FrameBuffer &operator=(FrameBuffer &&) = delete;

  virtual ~FrameBuffer();

  VkFramebuffer get_handle() const { return handle; }

  LogicalDevice &get_device() const { return device; }

  const VkExtent2D &get_extend() const { return extent; }

private:
  LogicalDevice &device;

  VkFramebuffer handle = VK_NULL_HANDLE;

  VkExtent2D extent{};
};
} // namespace cruelRender
} // namespace cruelEngine