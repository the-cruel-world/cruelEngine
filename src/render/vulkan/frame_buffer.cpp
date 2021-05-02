#include "frame_buffer.hpp"

#include "image_view.hpp"
#include "logical_device.hpp"
#include "render_pass.hpp"

namespace cruelEngine::cruelRender
{
FrameBuffer::FrameBuffer(LogicalDevice &device, const std::vector<ImageView> &imageView,
                         const VkExtent2D &extent, const RenderPass &renderPass) :
    device{device}, extent{extent}
{
    auto get_vkimage_view = [](const ImageView &imageView) { return imageView.get_handle(); };

    std::vector<VkImageView> vkImageViews(imageView.size(), VK_NULL_HANDLE);

    std::transform(imageView.begin(), imageView.end(), vkImageViews.begin(), get_vkimage_view);

    CRUEL_LOG("%s\n", "vkImageViews created.");

    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass      = renderPass.get_handle();
    framebufferInfo.attachmentCount = static_cast<u32>(vkImageViews.size());
    framebufferInfo.pAttachments    = vkImageViews.data();
    framebufferInfo.width           = extent.width;
    framebufferInfo.height          = extent.height;
    framebufferInfo.layers          = 1;

    VK_CHECK_RESULT(vkCreateFramebuffer(device.get_handle(), &framebufferInfo, nullptr, &handle));

    CRUEL_LOG("FrameBuffer Created %p\n", get_handle());
}
/*! \brief The FrameBuffer
 */
FrameBuffer::FrameBuffer(LogicalDevice &device, const std::vector<VkImageView> &imageView,
                         const VkExtent2D &extent, const RenderPass &renderPass) :
    device(device), extent(extent)
{
    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType           = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass      = renderPass.get_handle();
    framebufferInfo.attachmentCount = static_cast<u32>(imageView.size());
    framebufferInfo.pAttachments    = imageView.data();
    framebufferInfo.width           = extent.width;
    framebufferInfo.height          = extent.height;
    framebufferInfo.layers          = 1;

    VK_CHECK_RESULT(vkCreateFramebuffer(device.get_handle(), &framebufferInfo, nullptr, &handle));

    CRUEL_LOG("FrameBuffer Created %p\n", get_handle());
}

FrameBuffer::FrameBuffer(FrameBuffer &&other) :
    device{other.device}, handle{other.handle}, extent{other.extent}
{
    CRUEL_LOG("FrameBuffer moved %p\n", get_handle());
    other.handle = VK_NULL_HANDLE;
}

FrameBuffer::~FrameBuffer()
{
    CRUEL_LOG("FrameBuffer destroyed %p\n", get_handle());

    if (handle != VK_NULL_HANDLE)
        vkDestroyFramebuffer(device.get_handle(), handle, nullptr);
}

} // namespace cruelEngine::cruelRender
