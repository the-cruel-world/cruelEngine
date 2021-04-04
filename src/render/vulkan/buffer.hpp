#pragma once
#include "../vkcommon.h"

namespace cruelEngine
{
namespace cruelRender
{
class LogicalDevice;
class CommandBuffer;

uint32_t findMemoryType(const VkPhysicalDevice &device, uint32_t typeFilter,
                        VkMemoryPropertyFlags properties);

void copy_buffer(const VkDevice &device, const VkQueue &queue, const VkCommandPool &cmdPool,
                 VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

void create_buffer(const VkDevice &device, const VkPhysicalDevice &physicalDevice,
                   VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                   VkBuffer &buffer, VkDeviceMemory &bufferMemory);

class Buffer
{
public:
    Buffer(LogicalDevice &device);

    Buffer(LogicalDevice &device, VkDeviceSize bufferSize, VkBufferUsageFlags bufferUsage,
           VkMemoryPropertyFlags memoryProperties);

    Buffer(const Buffer &) = delete;

    Buffer(Buffer &&);

    Buffer &operator=(const Buffer &) = delete;

    Buffer &operator=(Buffer &&) = delete;

    ~Buffer();

    void createBuffer(VkDeviceSize _bufferSize, VkBufferUsageFlags _bufferUsage,
                      VkMemoryPropertyFlags _memoryProperties);

    void destroyBuffer();

    void load(void *data);

    void load(void *data, VkDeviceSize buffer_size);

    void load_stage(void *data, CommandBuffer &cmdBuffer);

    void load_stage(void *data, CommandBuffer &cmdBuffer, VkDeviceSize buffer_size);

    const VkBuffer &get_handle() const
    {
        return handle;
    }

    const VkDeviceMemory &get_handleMem() const
    {
        return memory;
    }

    const LogicalDevice &get_device() const
    {
        return device;
    }

    VkDeviceSize get_size() const
    {
        return buffer_size;
    }

    // VkBool32            map(VkDeviceSize size, VkDeviceSize offset);

    // void                unmap();

private:
    LogicalDevice &device;

    VkDeviceSize buffer_size = 0;

    // void                        *mapped;

    VkBuffer handle = VK_NULL_HANDLE;

    VkDeviceMemory memory = VK_NULL_HANDLE;

    VkBufferUsageFlags buffer_usage;

    VkMemoryPropertyFlags memory_flags;
};

class UniformBuffer
{
public:
    UniformBuffer(LogicalDevice &device, VkDeviceSize bufferSize);

    UniformBuffer(const UniformBuffer &) = delete;

    UniformBuffer(UniformBuffer &&);

    UniformBuffer &operator=(const UniformBuffer &) = delete;

    UniformBuffer &operator=(UniformBuffer &&) = delete;

    ~UniformBuffer();

    const VkBuffer &get_handle() const
    {
        return handle;
    }

    const VkDeviceMemory &get_handleMem() const
    {
        return memory;
    }

    void update(void *new_data);

private:
    LogicalDevice &device;

    VkDeviceSize buffer_size = 0;

    VkBuffer handle = VK_NULL_HANDLE;

    VkDeviceMemory memory = VK_NULL_HANDLE;

    VkBufferUsageFlags buffer_usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;

    VkMemoryPropertyFlags memory_flags =
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
};
} // namespace cruelRender
} // namespace cruelEngine