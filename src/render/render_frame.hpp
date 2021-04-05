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

class RenderFrame
{
public:
    RenderFrame(Image &image, ImageView &imageView, FrameBuffer &frameBuffer,
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

private:
    Image       image;
    ImageView   imageView;
    FrameBuffer frameBuffer;

    VkFence     fence          = VK_NULL_HANDLE;
    VkSemaphore renderFinished = VK_NULL_HANDLE;
    VkSemaphore imageAvailable = VK_NULL_HANDLE;

    RenderPass &   renderPass;
    CommandBuffer &commandBuffer;
};
} // namespace cruelEngine::cruelRender