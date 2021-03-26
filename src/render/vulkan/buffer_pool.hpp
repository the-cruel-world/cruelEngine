#pragma once
#include "../vkcommon.h"

namespace cruelEngine
{
namespace cruelRender
{
class LogicDevice;
class Buffer;

class BufferBlock
{
public:
    BufferBlock(LogicDevice &device, VkDeviceSize size, VkBufferUsageFlags usage,
                VkMemoryPropertyFlags memory_usage);

    ~BufferBlock();

private:
    LogicDevice &device;

    std::unique_ptr<Buffer> buffer;

    VkDeviceSize alignment = 0;

    VkDeviceSize offset = 0;
};

class BufferPool
{
public:
    BufferPool(LogicDevice &device);

    ~BufferPool();

private:
    LogicDevice &device;

    std::vector<std::unique_ptr<BufferBlock>> buffer_blocks;
};
} // namespace cruelRender
} // namespace cruelEngine