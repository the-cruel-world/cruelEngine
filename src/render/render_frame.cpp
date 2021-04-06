#include "render_frame.hpp"
#include "vulkan/command_buffer.hpp"
#include "vulkan/logical_device.hpp"
#include "vulkan/render_pass.hpp"

namespace cruelEngine::cruelRender
{
RenderFrame::RenderFrame(LogicalDevice &device, Image &image, ImageView &imageView,
                         FrameBuffer &frameBuffer, RenderPass &renderPass,
                         CommandBuffer &commandBuffer) :
    image{std::move(image)},
    imageView{std::move(imageView)},
    frameBuffer{std::move(frameBuffer)},
    device{device},
    renderPass{renderPass},
    commandBuffer{commandBuffer}
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

RenderFrame::~RenderFrame()
{
    commandBuffer.Release();
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

void RenderFrame::RecordBegin()
{
    VkExtent2D extent{image.GetExtent().width, image.GetExtent().height};
    commandBuffer.begin();
    commandBuffer.begin_renderpass(renderPass.get_handle(), frameBuffer.get_handle(), extent);
}

void RenderFrame::RecordEnd()
{
    commandBuffer.end_renderpass();
    commandBuffer.end();
}

void RenderFrame::Submit()
{
    // wait for the last submited image to finish it task.
    // vkWaitForFences(image.get_device().get_handle(), 1, &fence, VK_TRUE, UINT64_MAX);
    // vkResetFences(image.get_device().get_handle(), 1, &fence);
}

void RenderFrame::Present()
{}

CommandBuffer &RenderFrame::GetCommandBuffer()
{
    return commandBuffer;
}

VkSemaphore &RenderFrame::GetImageAvaiable()
{
    return imageAvailable;
}

VkSemaphore &RenderFrame::GetRenderFinished()
{
    return renderFinished;
}

VkFence &RenderFrame::GetFence()
{
    return fence;
}

void RenderFrame::WaitForFence(u64 timeout)
{
    vkWaitForFences(device.get_handle(), 1, &fence, VK_TRUE, UINT64_MAX);
}

void RenderFrame::ResetFence()
{
    vkResetFences(device.get_handle(), 1, &fence);
}

bool RenderFrame::GetStatus()
{
    return isrendering;
}

void RenderFrame::SetStatus(bool status)
{
    isrendering = status;
}

} // namespace cruelEngine::cruelRender