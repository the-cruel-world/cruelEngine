#include "descriptor.hpp"

#include "logical_device.hpp"

namespace cruelEngine
{
namespace cruelRender
{
DescriptorPool::DescriptorPool(LogicalDevice &device, const VkDescriptorPoolSize pool_size) :
    device{device}, pool_size{pool_size}
{
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes    = &pool_size;
    poolInfo.maxSets       = pool_size.descriptorCount;

    VK_CHECK_RESULT(vkCreateDescriptorPool(device.get_handle(), &poolInfo, nullptr, &handle));
}

DescriptorPool::DescriptorPool(LogicalDevice &device, const VkDescriptorPoolSize pool_size,
                               u32 maxSets) :
    device{device}, pool_size{pool_size}
{
    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes    = &pool_size;
    poolInfo.maxSets       = maxSets;

    VK_CHECK_RESULT(vkCreateDescriptorPool(device.get_handle(), &poolInfo, nullptr, &handle));
}

DescriptorPool::~DescriptorPool()
{
    if (handle != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorPool(device.get_handle(), handle, nullptr);
        handle = VK_NULL_HANDLE;
    }
}
DescriptorPool::DescriptorPool(DescriptorPool &&other) :
    device{other.device}, pool_size{other.pool_size}, handle{other.handle}
{
    other.handle = VK_NULL_HANDLE;
}

DescriptorSet::DescriptorSet(LogicalDevice &device, const DescriptorSetLayout &layout,
                             const DescriptorPool &pool) :
    device{device}, layout{layout}, pool{pool}
{
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType              = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool     = pool.get_handle();
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts        = &layout.get_handle();

    VK_CHECK_RESULT(vkAllocateDescriptorSets(device.get_handle(), &allocInfo, &handle));
}
/**
 * \brief desctuctor of descriptor pool, no need to destroy the desctiptor set
 * as it will be destroied by the pool
 */
DescriptorSet::~DescriptorSet()
{
    // if (handle != VK_NULL_HANDLE) {
    //   vkFreeDescriptorSets(device.get_handle(), pool.get_handle(), 1, &handle);
    // }
}

void DescriptorSet::update(VkDescriptorBufferInfo &bufferInfo)
{
    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType            = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet           = handle;
    descriptorWrite.dstBinding       = 0;
    descriptorWrite.dstArrayElement  = 0;
    descriptorWrite.descriptorType   = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount  = 1;
    descriptorWrite.pBufferInfo      = &bufferInfo;
    descriptorWrite.pImageInfo       = nullptr; // Optional
    descriptorWrite.pTexelBufferView = nullptr; // Optional

    vkUpdateDescriptorSets(device.get_handle(), 1, &descriptorWrite, 0, nullptr);
}
DescriptorSet::DescriptorSet(DescriptorSet &&other) :
    device{other.device}, layout{other.layout}, pool{other.pool}, handle{other.handle}
{
    other.handle = VK_NULL_HANDLE;
}

DescriptorSetLayout::DescriptorSetLayout(
    LogicalDevice &device, const std::vector<VkDescriptorSetLayoutBinding> new_bindings) :
    device{device}
{
    set_bindings(new_bindings);
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType        = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = bindings.size();
    layoutInfo.pBindings    = bindings.data();

    VK_CHECK_RESULT(
        vkCreateDescriptorSetLayout(device.get_handle(), &layoutInfo, nullptr, &handle));
}

DescriptorSetLayout::~DescriptorSetLayout()
{
    if (handle != VK_NULL_HANDLE)
    {
        vkDestroyDescriptorSetLayout(device.get_handle(), handle, nullptr);
        handle = VK_NULL_HANDLE;
    }
}

void DescriptorSetLayout::set_bindings(const std::vector<VkDescriptorSetLayoutBinding> new_bindings)
{
    for (auto binding : new_bindings)
    {
        bindings.push_back(binding);
    }
}
DescriptorSetLayout::DescriptorSetLayout(DescriptorSetLayout &&other) :
    device{other.device}, bindings{other.bindings}, handle{other.handle}
{
    other.handle = VK_NULL_HANDLE;
}
} // namespace cruelRender
} // namespace cruelEngine