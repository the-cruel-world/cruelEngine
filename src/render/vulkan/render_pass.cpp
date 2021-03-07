#include "render_pass.hpp"

#include "logical_device.hpp"

namespace cruelEngine {
namespace cruelRender {
RenderPass::RenderPass(LogicalDevice &_device,
                       const RenderPassAttachment &_attachments)
    : device(_device), attachments{_attachments} {
  VkRenderPassCreateInfo renderPassInfo{};
  renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  renderPassInfo.attachmentCount = 1;
  renderPassInfo.pAttachments = &(attachments.colorAttachment);
  renderPassInfo.subpassCount = attachments.subpass.size();
  renderPassInfo.pSubpasses = attachments.subpass.data();

  renderPassInfo.dependencyCount = 1;
  renderPassInfo.pDependencies = &(attachments.subpassDependency);

  VK_CHECK_RESULT(vkCreateRenderPass(device.get_handle(), &renderPassInfo,
                                     nullptr, &handle));
}

RenderPass::RenderPass(RenderPass &&other)
    : device{other.device}, attachments{other.attachments}, handle{
                                                                other.handle} {
  other.handle = VK_NULL_HANDLE;
}

RenderPass::~RenderPass() {
  if (handle != VK_NULL_HANDLE)
    vkDestroyRenderPass(device.get_handle(), handle, nullptr);
}
} // namespace cruelRender
} // namespace cruelEngine