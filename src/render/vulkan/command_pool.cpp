#include "command_pool.hpp"

#include "command_buffer.hpp"
#include "logical_device.hpp"

namespace cruelEngine::cruelRender
{
CommandPool::CommandPool(LogicalDevice &device, u32 queueFamilyIndex,
                         CommandBuffer::ResetModeFlags resetMode) :
    device{device}, queueFamilyIndex{queueFamilyIndex}, resetMode{resetMode}
{
    VkCommandPoolCreateFlags flags;

    switch (resetMode)
    {
        case CommandBuffer::ResetModeFlags::ResetIndividually:
        case CommandBuffer::ResetModeFlags::ReAllocate:
            flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
            break;

        case CommandBuffer::ResetModeFlags::ResetPool:
        default:
            flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT;
            break;
    }

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndex;
    poolInfo.flags            = flags;

    if (vkCreateCommandPool(device.get_handle(), &poolInfo, nullptr, &handle) != VK_SUCCESS)
    {
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

CommandBuffer &CommandPool::RequestCommandBuffer(const VkCommandBufferLevel &_level)
{
#ifdef RENDER_DEBUG
    fprintf(stdout, "[CommandPool] Pool %p: active primary command buffers: %d\n", this,
            active_primary_command_buffer_count);
    fprintf(stdout, "[CommandPool] Pool %p: active secondary command buffers: %d\n", this,
            active_secondary_command_buffer_count);
#endif

    // SortCommandBuffers();

    std::vector<std::unique_ptr<CommandBuffer>>::iterator command_it;

    auto isOccupied = [](std::unique_ptr<CommandBuffer> const &cmd) {
        return cmd->GetCmdState() == 0;
    };

    if (_level == VK_COMMAND_BUFFER_LEVEL_PRIMARY)
    {
        command_it = std::find_if(primary_command_buffers.begin(), primary_command_buffers.end(),
                                  isOccupied);
        if (command_it != primary_command_buffers.end())
        {
            (*command_it)->SetOccupied();
            return *(*command_it);
        }

        primary_command_buffers.push_back(
            std::make_unique<CommandBuffer>(*this, VK_COMMAND_BUFFER_LEVEL_PRIMARY));

        primary_command_buffers.back()->SetOccupied();
        return *primary_command_buffers.back();
    }
    else if (_level == VK_COMMAND_BUFFER_LEVEL_SECONDARY)
    {
        command_it = std::find_if(secondary_command_buffers.begin(),
                                  secondary_command_buffers.end(), isOccupied);
        if (command_it != secondary_command_buffers.end())
        {
            (*command_it)->SetOccupied();
            return *(*command_it);
        }
        secondary_command_buffers.push_back(
            std::make_unique<CommandBuffer>(*this, VK_COMMAND_BUFFER_LEVEL_SECONDARY));

        secondary_command_buffers.back()->SetOccupied();
        return *secondary_command_buffers.back();
    }
    else
    {
        throw std::runtime_error("Wrong command level. primary or secondary.");
    }
}

VkResult CommandPool::ResetPool()
{
    VkResult result = VK_SUCCESS;
    switch (resetMode)
    {
        case CommandBuffer::ResetModeFlags::ResetIndividually: {
            result = ResetCommandBuffers();

            break;
        }
        case CommandBuffer::ResetModeFlags::ResetPool: {
            result = vkResetCommandPool(device.get_handle(), handle, 0);

            if (result != VK_SUCCESS)
                return result;

            result = ResetCommandBuffers();

            break;
        }
        case CommandBuffer::ResetModeFlags::ReAllocate: {
            primary_command_buffers.clear();
            active_primary_command_buffer_count = 0;

            secondary_command_buffers.clear();
            active_secondary_command_buffer_count = 0;
            break;
        }
        default:
            throw std::runtime_error("[CommandPool] : Reset : Unknown reset mode.");
    }
    return VK_SUCCESS;
}

void CommandPool::test_list_commands()
{
    std::cout << "[primary commandbuffers] all:" << primary_command_buffers.size()
              << "\tactive: " << active_primary_command_buffer_count << std::endl;
    size_t i = 0;
    for (auto &cmdBuffer : primary_command_buffers)
    {
        std::cout << "\tid: " << i++;
        std::cout << "\taddr: " << cmdBuffer.get();
        std::cout << "\tStatus: " << cmdBuffer->GetCmdState() << std::endl;
    }
    // std::cout << std::endl;
    i = 0;
    std::cout << "[secondary commandbuffers] all:" << secondary_command_buffers.size()
              << "\tactive: " << active_secondary_command_buffer_count << std::endl;
    for (auto &cmdBuffer : secondary_command_buffers)
    {
        std::cout << "\tid: " << i++;
        std::cout << "\taddr: " << cmdBuffer.get();
        std::cout << "\tStatus: " << cmdBuffer->GetCmdState() << std::endl;
    }
    std::cout << std::endl;
}

VkResult CommandPool::ResetCommandBuffers()
{
    VkResult result = VK_SUCCESS;

    for (auto &cmdBuffer : primary_command_buffers)
    {
        result = cmdBuffer->reset(resetMode);
        if (result != VK_SUCCESS)
            return result;
    }
    active_primary_command_buffer_count = 0;

    for (auto &cmdBuffer : secondary_command_buffers)
    {
        result = cmdBuffer->reset(resetMode);
        if (result != VK_SUCCESS)
            return result;
    }
    active_secondary_command_buffer_count = 0;

    return result;
}
} // namespace cruelEngine::cruelRender