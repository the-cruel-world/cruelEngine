#pragma once
#include "../vkcommon.h"

namespace cruelEngine
{
namespace cruelRender
{
class LogicalDevice;

struct RenderPassAttachment
{
    std::vector<VkAttachmentDescription> colorAttachments;
    std::vector<VkSubpassDependency>     subpassDependencies;
    std::vector<VkSubpassDescription>    subpasses;
};

class RenderPass
{
public:
    RenderPass(LogicalDevice &device, const RenderPassAttachment &attachments);

    RenderPass(RenderPass &&other);

    RenderPass(const RenderPass &) = delete;

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

private:
    LogicalDevice &device;

    RenderPassAttachment attachments{};

    VkRenderPass handle = VK_NULL_HANDLE;
};
} // namespace cruelRender
} // namespace cruelEngine