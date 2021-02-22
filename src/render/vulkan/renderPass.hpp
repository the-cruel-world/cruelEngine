#pragma once
#include "vkcommon.h"

#include "vkDevice.hpp"
#include "swapChain.hpp"

namespace cruelEngine {
namespace VulkanContext {
    class RenderPass {
    public: 
        RenderPass(const VulkanDevice &_device);

        virtual ~RenderPass();

    public:

        void                    createRenderPass(const VkFormat &_colorFormat);

        void                    update(const VkFormat &_colorFormat);

        const VkRenderPass&     get_handle() const {return handle;}

        const VulkanDevice&     get_device() const {return device;}

    private:

        const VulkanDevice      &device;

        VkFormat                colorFormat {};

        VkRenderPass            handle = VK_NULL_HANDLE;

        void                    __create();

        void                    __destroy();
    };
}
}