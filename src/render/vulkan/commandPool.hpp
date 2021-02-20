#pragma once

#include "vkcommon.h"

#include "vkDevice.hpp"
#include "swapChain.hpp"
#include "pipeLine.hpp"
#include "renderPass.hpp"
#include "frameBuffer.hpp"

namespace cruelEngine {
namespace VulkanContext {
    /*! A wrap of vulkan command pool for easy management. */
    class CommandPool {
    public:
        /*! Create a command pool, it requires a physical device. */
        CommandPool (const VulkanDevice &_device);

        virtual ~CommandPool ();

        void                    createCommandPool();
        
        /*! Return the handle of the commandpool. */ 
        const VkCommandPool&    get_handle() const {return handle;}
        /*! Return a reference to the vulkandevice instance. */ 
        const VulkanDevice&     get_device() const {return device;}

    protected:

    private:
        const VulkanDevice    &device;

        VkCommandPool   handle = VK_NULL_HANDLE;
    };
}
}