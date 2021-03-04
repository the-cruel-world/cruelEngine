#include "frame_buffer.hpp"
#include "logical_device.hpp"
#include "render_pass.hpp"

namespace cruelEngine {
namespace cruelRender {
    /*! \brief The FrameBuffer
    */
    FrameBuffer::FrameBuffer(LogicalDevice &device, const VkImageView &imageView, const VkExtent2D &extent, const RenderPass &renderPass)
        : device (device), extent (extent)
    {
        VkFramebufferCreateInfo framebufferInfo{};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = renderPass.get_handle();
        framebufferInfo.attachmentCount = 1;
        framebufferInfo.pAttachments = &imageView;
        framebufferInfo.width = extent.width;
        framebufferInfo.height = extent.height;
        framebufferInfo.layers = 1;

        VK_CHECK_RESULT (vkCreateFramebuffer(device.get_handle(), &framebufferInfo, nullptr, &handle));
        //std::cout << "FrameBuffer created! id: " << get_handle() << std::endl;
    }

    FrameBuffer::~FrameBuffer(){
        //std::cout << "FrameBuffer Destroying!" << std::endl;
        if (handle != VK_NULL_HANDLE)
            //std::cout << "FrameBuffer Destroying!" << std::endl;
            vkDestroyFramebuffer(device.get_handle(), handle, nullptr);
            //std::cout << "FrameBuffer Destroyed!" << std::endl;
    }
}
}