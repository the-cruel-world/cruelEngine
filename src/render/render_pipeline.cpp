//
// Created by yiwen on 4/25/21.
//

#include "render/render_pipeline.hpp"
#include "render/render_task.hpp"
#include "render/subpass.hpp"

namespace cruelEngine::cruelRender
{
RenderPipeline::RenderPipeline(LogicalDevice &                         device,
                               std::vector<std::unique_ptr<SubPass>> &&subpasses) :
    subpasses{std::move(subpasses)}, device{device}
{
    BuildRenderPass();
}

void RenderPipeline::Draw(RenderTarget &target)
{}

void RenderPipeline::Prepare()
{}

void RenderPipeline::AddSubpass(std::unique_ptr<SubPass> &&subpass)
{
    subpasses.push_back(std::move(subpass));
}

std::vector<std::unique_ptr<SubPass>> &RenderPipeline::GetSubPasses()
{
    return subpasses;
}
const std::vector<std::unique_ptr<RenderTask>> &RenderPipeline::GetTasks() const
{
    return tasks;
}
RenderPass &RenderPipeline::GetRenderPass()
{
    return *renderPass;
}
std::unique_ptr<SubPass> &RenderPipeline::GetActiveSubpass()
{
    return subpasses.at(active_subpass_idx);
}
void RenderPipeline::BuildRenderPass()
{
    assert(subpasses.size() > 0 && "Can not create a renderpass without any subopass.");

    // collect renderpass infos
    std::vector<SubpassInfo> subpass_info(subpasses.size());
    auto                     subpass_info_it = subpass_info.begin();
    for (auto &subpass : subpasses)
    {
        subpass_info_it->input_attachments         = subpass->get_input_attachments();
        subpass_info_it->output_attachments        = subpass->get_output_attachments();
        subpass_info_it->color_resolve_attachments = subpass->get_color_resolve_attachments();
        subpass_info_it->disable_depth_stencil_attachment =
            subpass->get_disable_depth_stencil_attachment();
        subpass_info_it->depth_stencil_resolve_attachment =
            subpass->get_depth_stencil_resolve_attachment();
        subpass_info_it->depth_stencil_resolve_mode = subpass->get_depth_stencil_resolve_mode();
    }

    // build the final renderpass
    // create the render pass
    std::vector<VkAttachmentDescription> attachments;
    renderPass = std::make_unique<RenderPass>(device, attachments, subpass_info);
}
} // namespace cruelEngine::cruelRender