#include "frameBuffer.hpp"

namespace cruelEngine {
namespace VulkanContext {
    /*! \brief The FrameBuffer
    */
    FrameBuffer::FrameBuffer(const VulkanDevice &_device, const VkImageView &_imageView, const VkExtent2D _extent, const RenderPass &_renderPass)
        : device (_device), extent (_extent)
    {
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = _renderPass.get_handle();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = &_imageView;
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;

        VK_CHECK_RESULT (vkCreateFramebuffer(device.logicalDevice, &framebufferInfo, nullptr, &handle));
        //std::cout << "FrameBuffer created! id: " << get_handle() << std::endl;
    }

    FrameBuffer::FrameBuffer(const VulkanDevice &_device, const VkImageView &_imageView, const VkExtent2D _extent, const VkRenderPass &_renderPass)
        : device (_device), extent (_extent)
    {
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = _renderPass;
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = &_imageView;
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;

        VK_CHECK_RESULT (vkCreateFramebuffer(device.logicalDevice, &framebufferInfo, nullptr, &handle));
        //std::cout << "FrameBuffer created!" << std::endl;
    }

    FrameBuffer::~FrameBuffer(){
        //std::cout << "FrameBuffer Destroying!" << std::endl;
        if (handle != VK_NULL_HANDLE)
            //std::cout << "FrameBuffer Destroying!" << std::endl;
            vkDestroyFramebuffer(device.logicalDevice, handle, nullptr);
            //std::cout << "FrameBuffer Destroyed!" << std::endl;
    }
}
}