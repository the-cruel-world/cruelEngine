#include "render_pass.hpp"

#include "logical_device.hpp"

namespace cruelEngine
{
namespace cruelRender
{
RenderPass::RenderPass(LogicalDevice &device, std::vector<VkAttachmentDescription> &attachments,
                       std::vector<SubpassInfo> subpasses) :
    device{device}, attachments{attachments}
{
    VkRenderPassCreateInfo renderPassInfo{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
    renderPassInfo.pNext = nullptr;
    renderPassInfo.flags; // optional

    // create subpasses and dependencies for renderpass. and then create the render pass.
    {
        create_render_pass();

        renderPassInfo.subpassCount;
        renderPassInfo.pSubpasses;

        renderPassInfo.dependencyCount;
        renderPassInfo.pDependencies;

        renderPassInfo.attachmentCount = static_cast<u32>(this->attachments.size());
        renderPassInfo.pAttachments    = this->attachments.data();
    }

    VK_CHECK_RESULT(vkCreateRenderPass(device.get_handle(), &renderPassInfo, nullptr, &handle));
}

RenderPass::RenderPass(RenderPass &&other) :
    device{other.device}, attachments{other.attachments}, handle{other.handle}
{
    other.handle = VK_NULL_HANDLE;
}

RenderPass::~RenderPass()
{
    if (handle != VK_NULL_HANDLE)
        vkDestroyRenderPass(device.get_handle(), handle, nullptr);
}
void RenderPass::create_render_pass()
{
    /**
     * If no subpass if provided, create a default subpass.
     * */
    if (subpasses.empty())
    {
        VkSubpassDescription subpass_description;
        subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

        subpass_description.colorAttachmentCount;
        subpass_description.pColorAttachments;

        subpass_description.inputAttachmentCount;
        subpass_description.pInputAttachments;

        subpass_description.pResolveAttachments;
        subpass_description.pDepthStencilAttachment;
    }
}

} // namespace cruelRender
} // namespace cruelEngine