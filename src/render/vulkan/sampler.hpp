#pragma once

#include "../vkcommon.h"

namespace cruelEngine {
namespace cruelRender {
class LogicalDevice;

class Sampler {
public:
  Sampler(LogicalDevice &device, const VkSamplerCreateInfo &info);

  Sampler(const Sampler &) = delete;

  Sampler(Sampler &&sampler);

  Sampler &operator=(const Sampler &) = delete;

  Sampler &operator=(Sampler &&) = delete;

  ~Sampler();

  VkSampler get_handle() const { return handle; }

private:
  LogicalDevice &device;

  VkSampler handle = VK_NULL_HANDLE;
};
} // namespace cruelRender
} // namespace cruelEngine