#include "queue.hpp"
#include "logical_device.hpp"
#include "physical_device.hpp"
#include "command_buffer.hpp"

namespace cruelEngine {
namespace cruelRender {
    Queue::Queue(LogicalDevice &_device, 
                const u32 _family_index, 
                const u32 _index, 
                const VkQueueFamilyProperties _properties) :
        device (_device), family_index(_family_index), index(_index), properties(_properties)
    {
        vkGetDeviceQueue(device.get_handle(), family_index, index, &handle);
    }

    Queue::Queue(Queue &&other) :
        device {other.device},
        family_index {other.family_index},
        index {other.index},
        properties {other.properties},
        handle {other.handle}
    {
        other.handle        = VK_NULL_HANDLE;
        other.family_index  = 0;
        other.index         = 0;
        other.properties    = {};
    }

    VkResult Queue::submit(const std::vector<VkSubmitInfo> &submit_infos, VkFence fence)
    {
        return vkQueueSubmit(handle, u32(submit_infos.size()), submit_infos.data(), fence);
    }

    VkResult Queue::submit(const CommandBuffer &command_buffer, VkFence fence)
    {
        VkSubmitInfo submitInfo = {
            .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
            .commandBufferCount = 1,
            .pCommandBuffers = &command_buffer.get_handle()};

        return submit({submitInfo}, fence);
    }

    VkResult Queue::present(const VkPresentInfoKHR &presentInfo)
    {        
        return vkQueuePresentKHR(handle, &presentInfo);
    }

    VkBool32 Queue::can_present(const VkSurfaceKHR &surface)
    {
        VkBool32 can_present = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device.get_physicalDevice().get_handle(), family_index, surface, &can_present);
    }

    VkResult Queue::wait_idle()
    {
        return vkQueueWaitIdle(handle);
    }
}
}