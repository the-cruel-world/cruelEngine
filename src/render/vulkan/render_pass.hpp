#pragma once
#include "../vkcommon.h"

namespace cruelEngine
{
namespace cruelRender
{
class LogicalDevice;

struct SubpassInfo
{
    std::vector<uint32_t> input_attachments;

    std::vector<uint32_t> output_attachments;

    std::vector<uint32_t> color_resolve_attachments;

    bool disable_depth_stencil_attachment;

    uint32_t depth_stencil_resolve_attachment;

    VkResolveModeFlagBits depth_stencil_resolve_mode;
    // change it to a vulkan independent flag
};

struct RenderPassAttachment
{
    std::vector<VkAttachmentDescription> colorAttachments;
};

/**
 * Vulkan RenderPass
 *
 * 1. CreateInfo {
 *  1. flags
 *  2. attachmentCount
 *  3. pattachments --> VkAttachmentDescription
 *  4. subpassCount
 *  5. psubpasses --> VkSubpassDescription
 *  6. dependencyCount
 *  7. pdependencies
 *  };
 *
 * 2. VkRenderPassMultiviewCreateInfo
 * 3. VkAttachmentDescription
 * 4. VkSubpassDescription {
 *  1. pipelineBindPoint
 *  2. inputAttachmentCount
 *  3. pInputAttachments
 *  4. colorAttachmentCount
 *  5. pColorAttachments
 *  6. pResolveAttachments
 *  7. pDepthStencilAttachment
 *  8. preserveAttachmentCount
 *  9. pPreserveAttachments
 *  }
 *
 * */

class RenderPass
{
public:
    /**
     * RenderPass takes the jobe of both render pass and subpasses.
     *
     * @param device the physical device of it's parent.
     * @param attachments the color attachments of the render targets(the frame to ve rendered).
     * @param subpasses all the subpasses belongs to this render pass. If none is provided, create a
     * default subpass.
     * */
    RenderPass(LogicalDevice &device, std::vector<VkAttachmentDescription> &attachments,
               std::vector<SubpassInfo> subpasses);

    RenderPass(RenderPass &&other); // move operator

    RenderPass(const RenderPass &) = delete; // copy operation is disabled

    RenderPass &operator=(const RenderPass &) = delete;

    RenderPass &operator=(RenderPass &&) = delete;

    ~RenderPass();

public:
    const VkRenderPass &get_handle() const
    {
        return handle;
    }

    const LogicalDevice &get_device() const
    {
        return device;
    }

    void create_subpass(std::vector<VkAttachmentDescription> &attachments,
                        std::vector<SubpassInfo>              subpasses);

private:
    LogicalDevice &device;

    std::vector<VkAttachmentDescription> attachments{};

    VkRenderPass handle = VK_NULL_HANDLE;
};
} // namespace cruelRender
} // namespace cruelEngine