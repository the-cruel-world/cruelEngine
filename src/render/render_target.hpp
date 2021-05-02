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


struct LoadStoreInfo
{
    VkAttachmentLoadOp  loadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR;
    VkAttachmentStoreOp storeOp = VK_ATTACHMENT_STORE_OP_STORE;
};

struct Attachment
{
    VkFormat              format{VK_FORMAT_UNDEFINED};
    VkSampleCountFlagBits      samples{VK_SAMPLE_COUNT_1_BIT};
    VkImageUsageFlags     usage{VK_IMAGE_USAGE_SAMPLED_BIT};
    VkImageLayout         initLayout{VK_IMAGE_LAYOUT_UNDEFINED};
    VkImageLayout         finalLayout{VK_IMAGE_LAYOUT_UNDEFINED};

    Attachment(VkFormat format, VkSampleCountFlagBits samples, VkImageUsageFlags usage,
               VkImageLayout initLayout  = VK_IMAGE_LAYOUT_UNDEFINED,
               VkImageLayout finalLayout = VK_IMAGE_LAYOUT_UNDEFINED);
};

class RenderTarget
{
public:
    RenderTarget(LogicalDevice &device, std::vector<Image> &&images);

    ~RenderTarget();

    std::vector<ImageView> &GetViews();

    const VkExtent3D &GetExtent() const;

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

    const std::vector<Attachment> &GetAttachments() const;

    const std::vector<LoadStoreInfo> &get_load_store_op() const;

    void set_load_store_op(const std::vector<LoadStoreInfo> load_store_op);

    const std::vector<LoadStoreInfo> &get_stencil_load_store_op() const;

    void set_stencil_load_store_op(const std::vector<LoadStoreInfo> stencil_load_store_op);


private:
    LogicalDevice &device;

    VkExtent3D extent{};

    std::vector<Image> images;

    std::vector<ImageView> imageViews;

    std::vector<Attachment> attachments;

    std::vector<LoadStoreInfo> load_store_op;

    std::vector<LoadStoreInfo> stencil_load_store_op;

    VkFence     fence          = VK_NULL_HANDLE;
    VkSemaphore renderFinished = VK_NULL_HANDLE;
    VkSemaphore imageAvailable = VK_NULL_HANDLE;

    bool isrendering = false;
};
} // namespace cruelEngine::cruelRender