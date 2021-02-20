#include "commandPool.hpp"

namespace cruelEngine {
namespace VulkanContext {
    CommandPool::CommandPool(const VulkanDevice &_device) : device (_device) 
    {

    }

    void CommandPool::createCommandPool()
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = device.getGraphicsQueueIndex();
        poolInfo.flags = 0; // Optional

        if (vkCreateCommandPool(device.logicalDevice, &poolInfo, nullptr, &handle) != VK_SUCCESS) {
            throw std::runtime_error("createCommandPool: failed to create command pool!");
        }
    }

    CommandPool::~CommandPool()
    {
        if (handle != VK_NULL_HANDLE) 
        {
            vkDestroyCommandPool(device.logicalDevice, handle, nullptr);
        }
    }
}
}