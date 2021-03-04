#include "command_pool.hpp"
#include "command_buffer.hpp"
#include "logical_device.hpp"

namespace cruelEngine {
namespace cruelRender {
    CommandPool::CommandPool(LogicalDevice &_device) : device (_device) 
    {
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = device.get_suitable_graphics_queue(0).get_family_index();
        poolInfo.flags = 0; // Optional

        if (vkCreateCommandPool(device.get_handle(), &poolInfo, nullptr, &handle) != VK_SUCCESS) {
            throw std::runtime_error("createCommandPool: failed to create command pool!");
        }
    }

    CommandPool::~CommandPool()
    {
        primary_command_buffers.clear();
        secondary_command_buffers.clear();

        if (handle != VK_NULL_HANDLE) 
        {
            vkDestroyCommandPool(device.get_handle(), handle, nullptr);
        }
    }

    CommandBuffer& CommandPool::request_commandbuffer(const VkCommandBufferLevel &_level)
    {
        if (_level == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
        {
            if (active_primary_command_buffer_count < primary_command_buffers.size())
            {
                return *primary_command_buffers.at(active_primary_command_buffer_count++);
            }
            primary_command_buffers.push_back(std::make_unique<CommandBuffer>(*this, VK_COMMAND_BUFFER_LEVEL_PRIMARY));

            active_primary_command_buffer_count++;

            return *primary_command_buffers.back();
        } else if (_level == VK_COMMAND_BUFFER_LEVEL_SECONDARY) {
            if (active_secondary_command_buffer_count < secondary_command_buffers.size())
            {
                return *secondary_command_buffers.at(active_secondary_command_buffer_count++);
            }
            secondary_command_buffers.push_back(std::make_unique<CommandBuffer>(*this, VK_COMMAND_BUFFER_LEVEL_SECONDARY));

            active_secondary_command_buffer_count++;

            return *secondary_command_buffers.back();
        } else {
            throw std::runtime_error("Wrong command level. primary or secondary.");
        }
    }
}
}