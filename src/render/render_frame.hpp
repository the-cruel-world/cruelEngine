//
// Created by yiwen on 5/2/21.
//

#ifndef CRUELENGINE_RENDER_FRAME_HPP
#define CRUELENGINE_RENDER_FRAME_HPP
#pragma once
#include "vkcommon.h"

namespace cruelEngine::cruelRender
{
class RenderTarget;

class RenderFrame
{
public:
    RenderFrame() = default;

    ~RenderFrame() = default;

    void AddTargets(std::vector<std::unique_ptr<RenderTarget>> &&render_targets);

    const std::vector<std::unique_ptr<RenderTarget>> &GetTargets() const;

private:
    std::vector<std::unique_ptr<RenderTarget>> targets;
};
} // namespace cruelEngine::cruelRender
#endif // CRUELENGINE_RENDER_FRAME_HPP