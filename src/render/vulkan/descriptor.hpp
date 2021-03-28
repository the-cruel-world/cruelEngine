#pragma once
#include "../vkcommon.h"

namespace cruelEngine
{
namespace cruelRender
{
class LogicalDevice;
class DescriptorSetLayout;
class DescriptorPool;
class DescriptorSet;

/**
 * \brief Descriptor pool. Manages a vkdescriptor pool and is able to create
 * new descriptor set.
 */
class DescriptorPool
{
public:
    DescriptorPool(
        LogicalDevice &            device,
        const VkDescriptorPoolSize pool_size /*need one more parameter to specify maxsets*/);

    DescriptorPool(LogicalDevice &device, const VkDescriptorPoolSize pool_size, u32 maxSets);

    DescriptorPool(const DescriptorPool &) = delete;

    DescriptorPool(DescriptorPool &&) = default;

    DescriptorPool &operator=(const DescriptorPool &) = delete;

    DescriptorPool &operator=(DescriptorPool &&) = delete;

    ~DescriptorPool();

    const VkDescriptorPool &get_handle() const
    {
        return handle;
    }

    const VkDescriptorPoolSize &getPoolSize() const
    {
        return pool_size;
    }

private:
    LogicalDevice &device;

    VkDescriptorPoolSize pool_size;

    VkDescriptorPool handle = VK_NULL_HANDLE;
};

class DescriptorSet
{
public:
    DescriptorSet(LogicalDevice &device, DescriptorSetLayout &layout, DescriptorPool &pool);

    DescriptorSet(const DescriptorSet &) = delete;

    DescriptorSet(DescriptorSet &&other);

    DescriptorSet &operator=(const DescriptorSet &) = delete;

    DescriptorSet &operator=(DescriptorSet &&) = delete;

    ~DescriptorSet();

    const VkDescriptorSet &get_handle() const
    {
        return handle;
    }

    const DescriptorSetLayout &get_layout() const
    {
        return layout;
    }

    void update(VkDescriptorBufferInfo &bufferInfo);

private:
    LogicalDevice &device;

    DescriptorSetLayout &layout;

    DescriptorPool &pool;

    VkDescriptorSet handle = VK_NULL_HANDLE;
};

class DescriptorSetLayout
{
public:
    DescriptorSetLayout(LogicalDevice &                                 device,
                        const std::vector<VkDescriptorSetLayoutBinding> new_bindings);

    DescriptorSetLayout(const DescriptorSetLayout &) = delete;

    DescriptorSetLayout(DescriptorSetLayout &&other);

    DescriptorSetLayout &operator=(const DescriptorSetLayout &) = delete;

    DescriptorSetLayout &operator=(DescriptorSetLayout &&) = delete;

    ~DescriptorSetLayout();

    void set_bindings(const std::vector<VkDescriptorSetLayoutBinding> new_bindings);

    const std::vector<VkDescriptorSetLayoutBinding> get_bindings() const
    {
        return bindings;
    }

    const VkDescriptorSetLayout &get_handle() const
    {
        return handle;
    }

private:
    LogicalDevice &device;

    std::vector<VkDescriptorSetLayoutBinding> bindings;

    VkDescriptorSetLayout handle = VK_NULL_HANDLE;
};
} // namespace cruelRender
} // namespace cruelEngine