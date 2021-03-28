#include "gui_overlay.hpp"
namespace cruelEngine::cruelRender
{
GuiOverlay::GuiOverlay(RenderSession &session) : session{session}
{}

void GuiOverlay::Draw(CommandBuffer &commandBuffer)
{}

void GuiOverlay::newFrame()
{}

void GuiOverlay::prepare_pipeline()
{}
void GuiOverlay::prepare_resource()
{}
void GuiOverlay::free_resource()
{}
void GuiOverlay::destroy_pipeline()
{}
} // namespace cruelEngine::cruelRender