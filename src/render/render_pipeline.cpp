//
// Created by yiwen on 4/25/21.
//

#include "render/render_pipeline.hpp"
#include "render/render_target.hpp"
#include "render/render_task.hpp"
#include "render/subpass.hpp"
#include "render/vulkan/command_buffer.hpp"
#include "render/vulkan/frame_buffer.hpp"

namespace cruelEngine::cruelRender
{
RenderPipeline::RenderPipeline(LogicalDevice &                         device,
                               std::vector<std::unique_ptr<SubPass>> &&subpasses) :
    subpasses{std::move(subpasses)}, device{device}
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

    if (renderPass)
        renderPass.reset();
    BuildRenderPass(target);

    for (size_t i = 0; i < subpasses.size(); i++)
    {
        active_subpass_idx = i;

        auto &subpass = subpasses[i];

        // update subpass attachments

        if (i == 0)
        {
            // require framebuffer in resource cache
            VkExtent2D  extent2D = {target.GetExtent().width, target.GetExtent().height};
            FrameBuffer render_frame_buffer(device, target.GetViews(), extent2D, *renderPass);
            commandBuffer.begin_renderpass(renderPass->get_handle(),
                                           render_frame_buffer.get_handle(), extent2D);
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
RenderPass &RenderPipeline::GetRenderPass()
{
    return *renderPass;
}
std::unique_ptr<SubPass> &RenderPipeline::GetActiveSubpass()
{
    return subpasses.at(active_subpass_idx);
}

void RenderPipeline::BuildRenderPass(RenderTarget &target)
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
    auto targetAttachments = target.GetAttachments();
    auto load_store = target.get_load_store_op();
    auto stencil_load_store = target.get_stencil_load_store_op();

    std::vector<VkAttachmentDescription> attachments (targetAttachments.size());
    for (size_t i = 0; i < targetAttachments.size(); i++)
    {
        VkAttachmentDescription attachment{};

        attachment.format = targetAttachments[i].format;
        attachment.samples = targetAttachments[i].samples;
        attachment.initialLayout = targetAttachments[i].initLayout;
        attachment.finalLayout = targetAttachments[i].finalLayout;

        attachment.loadOp = load_store[i].loadOp;
        attachment.storeOp = load_store[i].storeOp;
        attachment.stencilLoadOp = stencil_load_store[i].loadOp;
        attachment.stencilStoreOp = stencil_load_store[i].storeOp;
    }
    renderPass = std::make_unique<RenderPass>(device, attachments, subpass_info);

    CRUEL_LOG("%s\n", "Pipeline subPasses created");
}

} // namespace cruelEngine::cruelRender