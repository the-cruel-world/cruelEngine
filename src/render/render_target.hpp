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

class RenderTarget
{
public:
    RenderTarget(LogicalDevice &device, Image &image, ImageView &imageView);
    ~RenderTarget();

    void RecordBegin(CommandBuffer &commandBuffer, RenderPass &renderPass);

    void RecordEnd(CommandBuffer &commandBuffer);

    void Submit();

    void Present();

    VkSemaphore &GetImageAvaiable();

    VkSemaphore &GetRenderFinished();

    VkFence &GetFence();

    void WaitForFence(u64 timeout);

    void ResetFence();

    bool GetStatus();

    void SetStatus(bool status);

private:
    Image     image;
    ImageView imageView;

    VkFence     fence          = VK_NULL_HANDLE;
    VkSemaphore renderFinished = VK_NULL_HANDLE;
    VkSemaphore imageAvailable = VK_NULL_HANDLE;

    LogicalDevice &device;

    bool isrendering = false;
};
} // namespace cruelEngine::cruelRender