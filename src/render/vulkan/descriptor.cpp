#include "descriptor.hpp"

#include "logical_device.hpp"

namespace cruelEngine {
namespace cruelRender {

DescriptorPool::DescriptorPool(LogicalDevice &device,
                               const VkDescriptorPoolSize pool_size)
    : device{device}, pool_size{pool_size} {}

DescriptorPool::~DescriptorPool() {
  if (handle != VK_NULL_HANDLE)
    vkDestroyDescriptorPool(device.get_handle(), handle, nullptr);
}

DescriptorSet::DescriptorSet(LogicalDevice &device, DescriptorSetLayout &layout,
                             DescriptorPool &pool)
    : device{device}, layout{layout}, pool{pool} {}

DescriptorSet::~DescriptorSet() {}

DescriptorSetLayout::DescriptorSetLayout(
    LogicalDevice &device,
    const std::vector<VkDescriptorSetLayoutBinding> new_bindings)
    : device{device} {
  set_bindings(new_bindings);
  VkDescriptorSetLayoutCreateInfo layoutInfo{};
  layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
  layoutInfo.bindingCount = bindings.size();
  layoutInfo.pBindings = bindings.data();

  VK_CHECK_RESULT(vkCreateDescriptorSetLayout(device.get_handle(), &layoutInfo,
                                              nullptr, &handle));
}

DescriptorSetLayout::~DescriptorSetLayout() {
  if (handle != VK_NULL_HANDLE)
    vkDestroyDescriptorSetLayout(device.get_handle(), handle, nullptr);
}

void DescriptorSetLayout::set_bindings(
    const std::vector<VkDescriptorSetLayoutBinding> new_bindings) {
  for (auto binding : new_bindings) {
    bindings.push_back(binding);
  }
}
} // namespace cruelRender
} // namespace cruelEngine