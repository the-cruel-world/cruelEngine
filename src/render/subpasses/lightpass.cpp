#include "render/subpasses/lightpass.hpp"

namespace cruelEngine::cruelRender
{
LightPass::LightPass(RenderSession &session) : SubPass(session)
{}

void LightPass::prepare()
{}
void LightPass::prepare_pipeline()
{}
void LightPass::draw(CommandBuffer &commandBuffer)
{}
} // namespace cruelEngine::cruelRender