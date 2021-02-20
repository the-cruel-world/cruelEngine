#pragma once

#include "vkcommon.h"

#include "vkDevice.hpp"
#include "swapChain.hpp"
#include "pipeLine.hpp"
#include "renderPass.hpp"

namespace cruelEngine {
namespace VulkanContext {
    struct FrameBuffer {
    public:
        FrameBuffer(const VulkanDevice &_device, const VkImageView& _imageView, const VkExtent2D _extent, const RenderPass &_renderPass);

        FrameBuffer(const VulkanDevice &_device, const VkImageView& _imageView, const VkExtent2D _extent, const VkRenderPass &_renderPass);

        // FrameBuffer(const FrameBuffer &&other);

        virtual ~FrameBuffer();

        const VkFramebuffer&        get_handle() const {return handle;}

        const VulkanDevice&         get_device() const {return device;}

    private:
        const VulkanDevice          &device;

        VkFramebuffer               handle = VK_NULL_HANDLE;

        VkExtent2D                  extent{};
    };
}
}