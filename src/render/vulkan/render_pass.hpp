#pragma once
#include "../vkcommon.h"

namespace cruelEngine {
namespace cruelRender {

    class LogicalDevice;

    struct RenderPassAttachment {
        VkAttachmentDescription colorAttachment;
        VkAttachmentReference   colorAttachmentRef;
        VkSubpassDependency     subpassDependency;
        std::vector<VkSubpassDescription> subpass;
    };

    class RenderPass {
    public: 
        RenderPass(LogicalDevice &device, const RenderPassAttachment &attachments);

        RenderPass(RenderPass &&other);

        RenderPass(const RenderPass &) = delete;

        RenderPass &operator=(const RenderPass &) = delete;

        RenderPass &operator=(RenderPass &&) = delete;

        ~RenderPass();

    public:
        const VkRenderPass&     get_handle() const {return handle;}

        const LogicalDevice&    get_device() const {return device;}

    private:

        LogicalDevice           &device;

        RenderPassAttachment    attachments {};

        VkRenderPass            handle = VK_NULL_HANDLE;
    };
}
}