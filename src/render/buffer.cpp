#include "buffer.hpp"

namespace realEngine {

    FrameBuffer::FrameBuffer(const VulkanDevice &_device, const SwapChain &_swapChain, const RenderPass &_renderPass)
        : device (_device), swapChain (_swapChain), renderPass (_renderPass)
    {}

    FrameBuffer::~FrameBuffer(){
        destroyFrameBuffer();
    }

    void FrameBuffer::createFrameBuffer()
    {
        //std::cout << "createFrameBuffer: Creating frame buffer" << std::endl;
        Framebuffers.resize(swapChain.imageViews.size());

        for (size_t i = 0; i < swapChain.imageViews.size(); i++) {
            VkImageView attachments[] = {
                swapChain.imageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo{};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = renderPass.renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = swapChain.extent2dMode.width;
            framebufferInfo.height = swapChain.extent2dMode.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(device.logicalDevice, &framebufferInfo, 
                        nullptr, &Framebuffers[i]) != VK_SUCCESS) {
                throw std::runtime_error("createFrameBuffer: failed to create framebuffer!");
            }
        }
    }
    void FrameBuffer::destroyFrameBuffer()
    {
        //std::cout << "destroyFrameBuffer: Cleaning frame buffer" << std::endl;
        for (auto framebuffer : Framebuffers) {
            vkDestroyFramebuffer(device.logicalDevice, framebuffer, nullptr);
    }
    }
}