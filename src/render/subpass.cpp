//
// Created by yiwen on 4/20/21.
//

#include "subpass.hpp"

namespace cruelEngine::cruelRender
{
SubPass::SubPass(RenderSession &session) : session{session}
{}

RenderSession &SubPass::GetSession()
{
    return session;
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
}; // namespace cruelEngine::cruelRender