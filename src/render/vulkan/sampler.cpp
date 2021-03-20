#include "sampler.hpp"
#include "logical_device.hpp"

namespace cruelEngine::cruelRender {
Sampler::Sampler(LogicalDevice &device, const VkSamplerCreateInfo &info)
    : device{device} {
  VK_CHECK_RESULT(
      vkCreateSampler(device.get_handle(), &info, nullptr, &handle));
}

Sampler::Sampler(Sampler &&other) : device{other.device}, handle{other.handle} {
  other.handle = VK_NULL_HANDLE;
}

Sampler::~Sampler() {
  if (handle != VK_NULL_HANDLE) {
    vkDestroySampler(device.get_handle(), handle, nullptr);
  }
}

VkSampler Sampler::get_handle() const {
  assert(handle != VK_NULL_HANDLE && "Sampler handle is invalid");
  return handle;
}

} // namespace cruelEngine::cruelRender