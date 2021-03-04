#pragma once
#include "../vkcommon.h"

namespace cruelEngine {
namespace cruelRender {

    class CommandBuffer;

    class LogicalDevice;

    /*! A wrap of vulkan command pool for easy management. */
    class CommandPool {
    public:
        /*! Create a command pool, it requires a physical device. */
        CommandPool (LogicalDevice &_device);

        virtual ~CommandPool ();

        // void                    createCommandPool();
        
        /*! Return the handle of the commandpool. */ 
        const VkCommandPool&    get_handle() const {return handle;}

        /*! Return a reference to the vulkandevice instance. */ 
        LogicalDevice&    get_device() const {return device;}

        /*! \brief Request a new commandbuffer from the commandPool. */
        CommandBuffer&          request_commandbuffer(const VkCommandBufferLevel &_level);

        VkResult                reset_pool();

    private:
        LogicalDevice           &device;

        VkCommandPool           handle = VK_NULL_HANDLE;

        size_t                  thread_index = 0;

        u32                     queue_family_index = 0;

        std::vector<std::unique_ptr<CommandBuffer>> primary_command_buffers;

        u32                     active_primary_command_buffer_count     = 0;

        std::vector<std::unique_ptr<CommandBuffer>> secondary_command_buffers;

        u32                     active_secondary_command_buffer_count   = 0;
    };
}
}