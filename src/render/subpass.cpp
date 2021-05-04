//
// Created by yiwen on 4/20/21.
//

#include "render/subpass.hpp"
#include "render/vulkan/render_pass.hpp"
#include "render_context.hpp"

namespace cruelEngine::cruelRender
{
SubPass::SubPass(RenderContext &context) : context{context}
{}

RenderContext &SubPass::GetContext()
{
    return context;
}
std::vector<uint32_t> SubPass::get_input_attachments()
{
    return input_attachments;
}
std::vector<uint32_t> SubPass::get_output_attachments()
{
    return output_attachments;
}
std::vector<uint32_t> SubPass::get_color_resolve_attachments()
{
    return color_resolve_attachments;
}
bool SubPass::get_disable_depth_stencil_attachment()
{
    return disable_depth_stencil_attachment;
}
uint32_t SubPass::get_depth_stencil_resolve_attachment()
{
    return depth_stencil_resolve_attachment;
}
VkResolveModeFlagBits SubPass::get_depth_stencil_resolve_mode()
{
    return depth_stencil_resolve_mode;
}
void SubPass::set_input_attachments(std::vector<uint32_t> input_attachments_)
{
    input_attachments = input_attachments_;
}
void SubPass::set_output_attachments(const std::vector<uint32_t> output_attachments_)
{
    output_attachments = output_attachments_;
}
void SubPass::set_color_resolve_attachments(const std::vector<uint32_t> color_resolve_attachments_)
{
    color_resolve_attachments = color_resolve_attachments_;
}
void SubPass::set_disable_depth_stencil_attachment(const bool disable_depth_stencil_attachment_)
{
    disable_depth_stencil_attachment = disable_depth_stencil_attachment_;
}
void SubPass::set_depth_stencil_resolve_attachment(const u32 depth_stencil_resolve_attachment_)
{
    depth_stencil_resolve_attachment = depth_stencil_resolve_attachment_;
}
void SubPass::set_depth_stencil_resolve_mode(
    const VkResolveModeFlagBits depth_stencil_resolve_mode_)
{
    depth_stencil_resolve_mode = depth_stencil_resolve_mode_;
}
}; // namespace cruelEngine::cruelRender