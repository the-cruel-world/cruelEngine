#include "render_pass.hpp"

#include "logical_device.hpp"

namespace cruelEngine
{
namespace cruelRender
{
RenderPass::RenderPass(LogicalDevice &                             device,
                       const std::vector<VkAttachmentDescription> &attachments,
                       const std::vector<SubpassInfo> &            subpasses) :
    device{device}
{
    CRUEL_LOG("RenderPass creating. attachments: %zu\n", attachments.size());
    // create subpasses and dependencies for renderpass. and then create the render pass.
    {
        create_subpass(attachments, subpasses);
    }
}

RenderPass::RenderPass(RenderPass &&other) : device{other.device}, handle{other.handle}
{
    other.handle = VK_NULL_HANDLE;
}

RenderPass::~RenderPass()
{
    if (handle != VK_NULL_HANDLE)
        vkDestroyRenderPass(device.get_handle(), handle, nullptr);
}
void RenderPass::create_subpass(const std::vector<VkAttachmentDescription> &attachments,
                                const std::vector<SubpassInfo>              subpasses)
{
    std::vector<VkSubpassDescription> subpass_descriptions{};

    std::vector<std::vector<VkAttachmentReference>> input_attachments{subpasses.size()};
    std::vector<std::vector<VkAttachmentReference>> color_attachments{subpasses.size()};
    std::vector<std::vector<VkAttachmentReference>> depth_attachments{subpasses.size()};
    std::vector<std::vector<VkAttachmentReference>> color_resolve_attachments{subpasses.size()};
    std::vector<std::vector<VkAttachmentReference>> depth_resolve_attachments{subpasses.size()};

    // \todo Add depthstencil support
    for (size_t i = 0; i < subpasses.size(); ++i)
    {
        auto &subpass = subpasses[i];

        for (auto &output : subpass.output_attachments)
        {
            auto initial_layout = attachments[output].initialLayout == VK_IMAGE_LAYOUT_UNDEFINED ?
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL :
                                      attachments[output].initialLayout;
            color_attachments[i].push_back((VkAttachmentReference){output, initial_layout});
        }
        for (auto &input : subpass.input_attachments)
        {
            auto initial_layout = attachments[input].initialLayout == VK_IMAGE_LAYOUT_UNDEFINED ?
                                      VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL :
                                      attachments[input].initialLayout;
            input_attachments[i].push_back((VkAttachmentReference){input, initial_layout});
        }
        for (auto resolve : subpass.color_resolve_attachments)
        {
            auto initial_layout = attachments[resolve].initialLayout == VK_IMAGE_LAYOUT_UNDEFINED ?
                                      VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL :
                                      attachments[resolve].initialLayout;
            color_resolve_attachments[i].push_back(
                (VkAttachmentReference){resolve, initial_layout});
        }

        VkSubpassDescription subpass_description{};
        subpass_description.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;

        subpass_description.inputAttachmentCount = static_cast<u32>(input_attachments[i].size());
        subpass_description.pInputAttachments =
            input_attachments[i].empty() ? nullptr : input_attachments[i].data();

        subpass_description.colorAttachmentCount = static_cast<u32>(color_attachments[i].size());
        subpass_description.pColorAttachments =
            color_attachments[i].empty() ? nullptr : color_attachments[i].data();

        subpass_description.pResolveAttachments =
            color_resolve_attachments[i].empty() ? nullptr : color_resolve_attachments[i].data();

        subpass_descriptions.push_back(subpass_description);
    }

    /**
     * If no subpass if provided, create a default subpass.
     * */
    if (subpasses.empty())
    {
        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass_description{};

        subpass_description.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass_description.colorAttachmentCount = 1;
        subpass_description.pColorAttachments    = &colorAttachmentRef;

        subpass_descriptions.push_back(subpass_description);
    }
    u32 subpass_count = std::max<size_t>(1, subpasses.size()); // there will be at least one subpass
    std::vector<VkSubpassDependency> subpass_dependencies(subpass_count - 1);
    if (subpass_count > 1)
    {
        for (u32 i = 0; i < (u32)(subpass_dependencies.size()); i++)
        {
            subpass_dependencies[i].srcSubpass      = i;
            subpass_dependencies[i].dstSubpass      = i + 1;
            subpass_dependencies[i].srcStageMask    = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            subpass_dependencies[i].dstStageMask    = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
            subpass_dependencies[i].srcAccessMask   = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            subpass_dependencies[i].dstAccessMask   = VK_ACCESS_INPUT_ATTACHMENT_READ_BIT;
            subpass_dependencies[i].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;
        }
    }
    else if (subpass_count == 1)
    {
        VkSubpassDependency dependency{};
        dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass    = 0;
        dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        subpass_dependencies.push_back(std::move(dependency));
    }

    VkRenderPassCreateInfo renderPassInfo{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};

    renderPassInfo.subpassCount = static_cast<u32>(subpass_descriptions.size());
    renderPassInfo.pSubpasses   = subpass_descriptions.data();

    renderPassInfo.dependencyCount = static_cast<u32>(subpass_dependencies.size());
    renderPassInfo.pDependencies   = subpass_dependencies.data();

    renderPassInfo.attachmentCount = static_cast<u32>(attachments.size());
    renderPassInfo.pAttachments    = attachments.data();

    printf("subpassCount: %d\n", renderPassInfo.subpassCount);
    printf("subpasses: %p\n", renderPassInfo.pSubpasses);
    printf("dependencyCount: %d\n", renderPassInfo.dependencyCount);
    printf("dependencies: %p\n", renderPassInfo.pDependencies);
    printf("attachmentCount: %d\n", renderPassInfo.attachmentCount);
    printf("attachments: %p\n", renderPassInfo.pAttachments);

    VK_CHECK_RESULT(vkCreateRenderPass(device.get_handle(), &renderPassInfo, nullptr, &handle));
}

} // namespace cruelRender
} // namespace cruelEngine