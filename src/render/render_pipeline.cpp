//
// Created by yiwen on 4/25/21.
//

#include "render/render_pipeline.hpp"
#include "render/render_context.hpp"
#include "render/render_target.hpp"
#include "render/render_task.hpp"
#include "render/resource_cache.hpp"
#include "render/subpass.hpp"
#include "render/vulkan/command_buffer.hpp"
#include "render/vulkan/frame_buffer.hpp"
#include "render/vulkan/render_pass.hpp"

namespace cruelEngine::cruelRender
{
RenderPipeline::RenderPipeline(RenderContext &                         context,
                               std::vector<std::unique_ptr<SubPass>> &&subpasses) :
    subpasses{std::move(subpasses)}, context{context}
{
    //    BuildRenderPass();

    CRUEL_LOG("%s\n", "Pipeline Created");
}

RenderPipeline::~RenderPipeline()
{
    CRUEL_LOG("RenderPipeline: %p destroyed\n", this);
}

void RenderPipeline::Draw(CommandBuffer &commandBuffer, RenderTarget &target,
                          VkSubpassContents contents)
{
    assert(!subpasses.empty() && "RenderPipeline should contain at least one subpass.");

    RenderPass &renderPass = BuildRenderPass(target);

    for (size_t i = 0; i < subpasses.size(); i++)
    {
        active_subpass_idx = i;

        auto &subpass = subpasses[i];

        // update subpass attachments

        if (i == 0)
        {
            // require framebuffer in resource cache
            VkExtent2D extent2D = {target.GetExtent().width, target.GetExtent().height};

            FrameBuffer &frame_buffer = context.GetResourceCache().request_framebuffer(
                target.GetVkViews(), extent2D, renderPass);
            commandBuffer.begin_renderpass(renderPass.get_handle(), frame_buffer.get_handle(),
                                           extent2D);
        }
        else
        {
            // commandBuffer.nextSubpass();
        }

        subpass->draw(commandBuffer);
    }
    active_subpass_idx = 0;
}

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
std::unique_ptr<SubPass> &RenderPipeline::GetActiveSubpass()
{
    return subpasses.at(active_subpass_idx);
}

RenderPass &RenderPipeline::BuildRenderPass(RenderTarget &target)
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
    auto targetAttachments  = target.GetAttachments();
    auto load_store         = target.get_load_store_op();
    auto stencil_load_store = target.get_stencil_load_store_op();

    std::vector<VkAttachmentDescription> attachments(targetAttachments.size());
    for (size_t i = 0; i < targetAttachments.size(); i++)
    {
        attachments[i].format  = targetAttachments[i].format;
        attachments[i].samples = targetAttachments[i].samples;

        attachments[i].initialLayout = targetAttachments[i].initLayout;
        attachments[i].finalLayout   = targetAttachments[i].finalLayout;

        attachments[i].loadOp         = load_store[i].loadOp;
        attachments[i].storeOp        = load_store[i].storeOp;
        attachments[i].stencilLoadOp  = stencil_load_store[i].loadOp;
        attachments[i].stencilStoreOp = stencil_load_store[i].storeOp;
    }
    RenderPass &renderPass =
        context.GetResourceCache().request_render_pass(attachments, subpass_info);

    CRUEL_LOG("%s\n", "Pipeline subPasses created");

    return renderPass;
}

} // namespace cruelEngine::cruelRender