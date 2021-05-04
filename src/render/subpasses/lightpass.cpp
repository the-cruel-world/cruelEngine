#include "render/subpasses/lightpass.hpp"

namespace cruelEngine::cruelRender
{
LightPass::LightPass(RenderContext &context) : SubPass(context)
{}

void LightPass::prepare()
{}
void LightPass::prepare_pipeline()
{}
void LightPass::draw(CommandBuffer &commandBuffer)
{}
} // namespace cruelEngine::cruelRender