#include "render_target.hpp"
#include "vulkan/command_buffer.hpp"
#include "vulkan/frame_buffer.hpp"
#include "vulkan/image.hpp"
#include "vulkan/image_view.hpp"
#include "vulkan/logical_device.hpp"
#include "vulkan/render_pass.hpp"

namespace cruelEngine::cruelRender
{
Attachment::Attachment(VkFormat format, VkSampleCountFlagBits samples, VkImageUsageFlags usage,
                       VkImageLayout initLayout, VkImageLayout finalLayout) :
    format{format}, samples{samples}, usage{usage}, initLayout{initLayout}, finalLayout{finalLayout}
{}

RenderTarget::RenderTarget(LogicalDevice &device, std::vector<Image> &&images_) :
    device{device}, images{std::move(images_)}
{
    for (auto &image : images)
    {
        imageViews.push_back(std::move(ImageView(device, &image, image.get_format())));
        CRUEL_LOG("RenderTarget image: %p, imageView: %p\n", image.get_handle(),
                  imageViews.back().get_handle());

        attachments.push_back(
            Attachment(image.get_format(), image.GetSampleCount(), image.GetUsage()));
    }
    extent = images.at(0).GetExtent();

    CRUEL_LOG("RenderTarget: %p created\n", this);
}

RenderTarget::~RenderTarget()
{
    CRUEL_LOG("RenderTarget: %p destroyed\n", this);
}

void RenderTarget::RecordBegin(CommandBuffer &commandBuffer, RenderPass &renderPass)
{
    //    VkExtent2D extent{image.GetExtent().width, image.GetExtent().height};
    //
    //    // \todo should use framebuffer from resource cache instead;
    //    FrameBuffer frameBuffer(device, {imageView.get_handle()}, extent, renderPass);
    //    commandBuffer.begin();
    //    commandBuffer.begin_renderpass(renderPass.get_handle(), frameBuffer.get_handle(), extent);
}

void RenderTarget::RecordEnd(CommandBuffer &commandBuffer)
{
    commandBuffer.end_renderpass();
    commandBuffer.end();
}

void RenderTarget::Submit()
{
    // wait for the last submited image to finish it task.
    // vkWaitForFences(image.get_device().get_handle(), 1, &fence, VK_TRUE, UINT64_MAX);
    // vkResetFences(image.get_device().get_handle(), 1, &fence);
}

void RenderTarget::Present()
{}

VkSemaphore &RenderTarget::GetImageAvaiable()
{
    return imageAvailable;
}

VkSemaphore &RenderTarget::GetRenderFinished()
{
    return renderFinished;
}

VkFence &RenderTarget::GetFence()
{
    return fence;
}

void RenderTarget::WaitForFence(u64 timeout)
{
    vkWaitForFences(device.get_handle(), 1, &fence, VK_TRUE, UINT64_MAX);
}

void RenderTarget::ResetFence()
{
    vkResetFences(device.get_handle(), 1, &fence);
}

bool RenderTarget::GetStatus()
{
    return isrendering;
}

void RenderTarget::SetStatus(bool status)
{
    isrendering = status;
}
const VkExtent3D &RenderTarget::GetExtent() const
{
    return extent;
}
std::vector<ImageView> &RenderTarget::GetViews()
{
    return imageViews;
}
const std::vector<Attachment> &RenderTarget::GetAttachments() const
{
    return attachments;
}
const std::vector<LoadStoreInfo> &RenderTarget::get_load_store_op() const
{
    return load_store_op;
}
const std::vector<LoadStoreInfo> &RenderTarget::get_stencil_load_store_op() const
{
    return stencil_load_store_op;
}
void RenderTarget::set_load_store_op(const std::vector<LoadStoreInfo> load_store_op_)
{
    load_store_op = load_store_op_;
}
void RenderTarget::set_stencil_load_store_op(
    const std::vector<LoadStoreInfo> stencil_load_store_op_)
{
    stencil_load_store_op = stencil_load_store_op_;
}

} // namespace cruelEngine::cruelRender