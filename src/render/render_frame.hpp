#pragma once
#include "vkcommon.h"
#include "vulkan/frame_buffer.hpp"
#include "vulkan/image.hpp"
#include "vulkan/image_view.hpp"

namespace cruelEngine::cruelRender
{
class Image;
class ImageView;
class RenderPass;
class CommandBuffer;
class FrameBuffer;
class LogicalDevice;

class RenderFrame
{
public:
    RenderFrame(LogicalDevice &device, Image &image, ImageView &imageView, FrameBuffer &frameBuffer,
                RenderPass &renderPass, CommandBuffer &commandBuffer);
    ~RenderFrame();

    void RecordBegin();

    void RecordEnd();

    void Submit();

    void Present();

    CommandBuffer &GetCommandBuffer();

    VkSemaphore &GetImageAvaiable();

    VkSemaphore &GetRenderFinished();

    VkFence &GetFence();

    void WaitForFence(u64 timeout);

    void ResetFence();

    bool GetStatus();

    void SetStatus(bool status);

private:
    Image       image;
    ImageView   imageView;
    FrameBuffer frameBuffer;

    VkFence     fence          = VK_NULL_HANDLE;
    VkSemaphore renderFinished = VK_NULL_HANDLE;
    VkSemaphore imageAvailable = VK_NULL_HANDLE;

    LogicalDevice &device;
    RenderPass &   renderPass;
    CommandBuffer &commandBuffer;

    bool isrendering = false;
};
} // namespace cruelEngine::cruelRender