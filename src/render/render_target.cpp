#include "render_target.hpp"
#include "vulkan/command_buffer.hpp"
#include "vulkan/frame_buffer.hpp"
#include "vulkan/logical_device.hpp"
#include "vulkan/render_pass.hpp"

namespace cruelEngine::cruelRender
{
RenderTarget::RenderTarget(LogicalDevice &device, Image &image, ImageView &imageView) :
    image{std::move(image)}, imageView{std::move(imageView)}, device{device}
{
    // Create fence;
    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
    VK_CHECK_RESULT(vkCreateFence(image.get_device().get_handle(), &fenceInfo, nullptr, &fence));

    // create semaphore;
    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    VK_CHECK_RESULT(vkCreateSemaphore(image.get_device().get_handle(), &semaphoreInfo, nullptr,
                                      &imageAvailable));
    VK_CHECK_RESULT(vkCreateSemaphore(image.get_device().get_handle(), &semaphoreInfo, nullptr,
                                      &renderFinished));
}

RenderTarget::~RenderTarget()
{
    if (fence != VK_NULL_HANDLE)
    {
        vkDestroyFence(image.get_device().get_handle(), fence, nullptr);
    }
    if (imageAvailable != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(image.get_device().get_handle(), imageAvailable, nullptr);
    }
    if (renderFinished != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(image.get_device().get_handle(), renderFinished, nullptr);
    }
}

void RenderTarget::RecordBegin(CommandBuffer &commandBuffer, RenderPass &renderPass)
{
    VkExtent2D extent{image.GetExtent().width, image.GetExtent().height};

    // \todo should use framebuffer from resource cache instead;
    FrameBuffer frameBuffer(device, {imageView.get_handle()}, extent, renderPass);
    commandBuffer.begin();
    commandBuffer.begin_renderpass(renderPass.get_handle(), frameBuffer.get_handle(), extent);
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

} // namespace cruelEngine::cruelRender