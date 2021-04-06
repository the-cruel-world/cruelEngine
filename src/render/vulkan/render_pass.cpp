#include "render_pass.hpp"

#include "logical_device.hpp"

namespace cruelEngine
{
namespace cruelRender
{
RenderPass::RenderPass(LogicalDevice &_device, const RenderPassAttachment &_attachments) :
    device(_device), attachments{_attachments}
{
    VkRenderPassCreateInfo renderPassInfo{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
    renderPassInfo.pNext = nullptr;
    renderPassInfo.flags; // optional

    renderPassInfo.attachmentCount = static_cast<u32>(attachments.colorAttachments.size());
    renderPassInfo.pAttachments    = attachments.colorAttachments.data();

    renderPassInfo.subpassCount = static_cast<u32>(attachments.subpasses.size());
    renderPassInfo.pSubpasses   = attachments.subpasses.data();

    renderPassInfo.dependencyCount = static_cast<u32>(attachments.subpassDependencies.size());
    renderPassInfo.pDependencies   = attachments.subpassDependencies.data();

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
} // namespace cruelRender
} // namespace cruelEngine