#pragma once
#include "render/subpass.hpp"

namespace cruelEngine::cruelRender
{
class RenderSession;
class CommandBuffer;

class LightPass : public SubPass
{
public:
    LightPass(RenderContext &context);

    virtual void prepare() override;

    virtual void prepare_pipeline();

    virtual void draw(CommandBuffer &commandBuffer) override;

private:
};
} // namespace cruelEngine::cruelRender