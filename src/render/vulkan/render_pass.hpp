#pragma once
#include "vkcommon.h"

namespace cruelEngine {
namespace VulkanContext {

    class LogicalDevice;

    struct RenderPassAttachment {
        VkAttachmentDescription colorAttachment;
        VkAttachmentReference   colorAttachmentRef;
        VkSubpassDependency     subpassDependency;
    };

    class RenderPass {
    public: 
        RenderPass(LogicalDevice &device, const VkFormat &colorFormat);

        RenderPass(RenderPass &&other);

        RenderPass(const RenderPass &) = delete;

        RenderPass &operator=(const RenderPass &) = delete;

        RenderPass &operator=(RenderPass &&) = delete;

        ~RenderPass();

    public:

        void                    createRenderPass(const VkFormat &_colorFormat);

        const VkRenderPass&     get_handle() const {return handle;}

        const LogicalDevice&    get_device() const {return device;}

    private:

        LogicalDevice           &device;

        VkRenderPass            handle = VK_NULL_HANDLE;
    };
}
}