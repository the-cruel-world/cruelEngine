#include "buffer.hpp"

#include "command_buffer.hpp"
#include "logical_device.hpp"
#include "physical_device.hpp"
#include "queue.hpp"

namespace cruelEngine {
namespace cruelRender {

uint32_t findMemoryType(const VkPhysicalDevice &device, uint32_t typeFilter,
                        VkMemoryPropertyFlags properties) {
  VkPhysicalDeviceMemoryProperties memProperties;
  vkGetPhysicalDeviceMemoryProperties(device, &memProperties);
  for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
    if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags &
                                    properties) == properties) {
      return i;
    }
  }
  throw std::runtime_error("failed to find suitable memory type!");
}
void create_buffer(const VkDevice &device,
                   const VkPhysicalDevice &physicalDevice, VkDeviceSize size,
                   VkBufferUsageFlags usage, VkMemoryPropertyFlags properties,
                   VkBuffer &buffer, VkDeviceMemory &bufferMemory) {
  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = size;
  bufferInfo.usage = usage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
    throw std::runtime_error("failed to create buffer!");
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex = findMemoryType(
      physicalDevice, memRequirements.memoryTypeBits, properties);

  if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to allocate buffer memory!");
  }

  vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

void copy_buffer(const VkDevice &device, const VkQueue &queue,
                 const VkCommandPool &cmdPool, VkBuffer srcBuffer,
                 VkBuffer dstBuffer, VkDeviceSize size) {
  VkCommandBufferAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  allocInfo.commandPool = cmdPool;
  allocInfo.commandBufferCount = 1;

  VkCommandBuffer commandBuffer;
  vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

  VkCommandBufferBeginInfo beginInfo{};
  beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  vkBeginCommandBuffer(commandBuffer, &beginInfo);

  VkBufferCopy copyRegion{};
  copyRegion.srcOffset = 0; // Optional
  copyRegion.dstOffset = 0; // Optional
  copyRegion.size = size;
  vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
  vkEndCommandBuffer(commandBuffer);

  VkSubmitInfo submitInfo{};
  submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submitInfo.commandBufferCount = 1;
  submitInfo.pCommandBuffers = &commandBuffer;

  vkQueueSubmit(queue, 1, &submitInfo, VK_NULL_HANDLE);
  vkQueueWaitIdle(queue);
  vkFreeCommandBuffers(device, cmdPool, 1, &commandBuffer);
}

Buffer::Buffer(LogicalDevice &_device) : device(_device) {}

Buffer::Buffer(LogicalDevice &device, VkDeviceSize bufferSize,
               VkBufferUsageFlags bufferUsage,
               VkMemoryPropertyFlags memoryProperties)
    : Buffer(device) {
  memory_flags = memoryProperties;
  buffer_usage = bufferUsage;
  if (memory_flags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
    if (!(buffer_usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT))
      buffer_usage |= VK_BUFFER_USAGE_TRANSFER_DST_BIT;
  }
  device.get_physicalDevice();
  createBuffer(bufferSize, buffer_usage, memory_flags);
}

Buffer::~Buffer() { destroyBuffer(); }

void Buffer::createBuffer(VkDeviceSize _bufferSize,
                          VkBufferUsageFlags _bufferUsage,
                          VkMemoryPropertyFlags _memoryProperties) {
  buffer_size = _bufferSize;
  VkBufferCreateInfo bufferInfo{};
  bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  bufferInfo.size = buffer_size;
  bufferInfo.usage = _bufferUsage;
  bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

  if (vkCreateBuffer(device.get_handle(), &bufferInfo, nullptr, &handle) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create buffer!");
  }

  VkMemoryRequirements memRequirements;
  vkGetBufferMemoryRequirements(device.get_handle(), handle, &memRequirements);

  VkMemoryAllocateInfo allocInfo{};
  allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
  allocInfo.allocationSize = memRequirements.size;
  allocInfo.memoryTypeIndex =
      findMemoryType(device.get_physicalDevice().get_handle(),
                     memRequirements.memoryTypeBits, _memoryProperties);

  if (vkAllocateMemory(device.get_handle(), &allocInfo, nullptr, &memory) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to allocate buffer memory!");
  }
  vkBindBufferMemory(device.get_handle(), handle, memory, 0);
}

// VkBool32 Buffer::map(VkDeviceSize size, VkDeviceSize offset)
// {
//     return vkMapMemory(device.get_handle(), memory, offset, size, 0,
//     &mapped);
// }

// void Buffer::unmap()
// {
//     if (mapped)
//     {
//         vkUnmapMemory(device.get_handle(), memory);
//         mapped = nullptr;
//     }
// }

void Buffer::destroyBuffer() {
  if (handle != VK_NULL_HANDLE)
    vkDestroyBuffer(device.get_handle(), handle, nullptr);
  if (memory != VK_NULL_HANDLE)
    vkFreeMemory(device.get_handle(), memory, nullptr);
}

void Buffer::load(void *data) {
  if (buffer_usage & VK_BUFFER_USAGE_TRANSFER_DST_BIT) {
    load_stage(data,
               device.request_commandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY));
    return;
  }
  void *_data;
  vkMapMemory(device.get_handle(), memory, 0, buffer_size, 0, &_data);
  memcpy(_data, data, (size_t)buffer_size);
  vkUnmapMemory(device.get_handle(), memory);
}

void Buffer::load_stage(void *data, CommandBuffer &cmdBuffer) {
  VkBuffer staging_buffer = VK_NULL_HANDLE;
  VkDeviceMemory staging_buffer_memory = VK_NULL_HANDLE;
  create_buffer(device.get_handle(), device.get_physicalDevice().get_handle(),
                buffer_size, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
                    VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                staging_buffer, staging_buffer_memory);

  void *temp_data;
  vkMapMemory(device.get_handle(), staging_buffer_memory, 0, buffer_size, 0,
              &temp_data);
  memcpy(temp_data, data, buffer_size);
  vkUnmapMemory(device.get_handle(), staging_buffer_memory);

  cmdBuffer.beginOneTime();
  VkBufferCopy copyRegion{};
  copyRegion.srcOffset = 0; // Optional
  copyRegion.dstOffset = 0; // Optional
  copyRegion.size = buffer_size;
  vkCmdCopyBuffer(cmdBuffer.get_handle(), staging_buffer, handle, 1,
                  &copyRegion);
  cmdBuffer.endOneTime();

  vkDestroyBuffer(device.get_handle(), staging_buffer, nullptr);
  vkFreeMemory(device.get_handle(), staging_buffer_memory, nullptr);
}

UniformBuffer::UniformBuffer(LogicalDevice &device, VkDeviceSize bufferSize)
    : device{device}, buffer_size{bufferSize} {
  create_buffer(device.get_handle(), device.get_physicalDevice().get_handle(),
                buffer_size, buffer_usage, memory_flags, handle, memory);
}

UniformBuffer::~UniformBuffer() {
  if (handle != VK_NULL_HANDLE)
    vkDestroyBuffer(device.get_handle(), handle, nullptr);
  if (memory != VK_NULL_HANDLE)
    vkFreeMemory(device.get_handle(), memory, nullptr);
}

void UniformBuffer::update(void *new_data) {
  void *data;
  vkMapMemory(device.get_handle(), memory, 0, buffer_size, 0, &data);
  memcpy(data, new_data, buffer_size);
  vkUnmapMemory(device.get_handle(), memory);
}
} // namespace cruelRender
} // namespace cruelEngine