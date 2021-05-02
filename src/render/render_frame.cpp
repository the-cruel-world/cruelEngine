//
// Created by yiwen on 5/2/21.
//

#include "render/render_frame.hpp"
#include "render/render_target.hpp"

namespace cruelEngine::cruelRender
{
void RenderFrame::AddTargets(std::vector<std::unique_ptr<RenderTarget>> &&render_targets)
{
    targets = std::move(render_targets);
}
const std::vector<std::unique_ptr<RenderTarget>> &RenderFrame::GetTargets() const
{
    return targets;
}
} // namespace cruelEngine::cruelRender