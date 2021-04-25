#include "gui_overlay.hpp"
namespace cruelEngine::cruelRender
{
GuiOverlay::GuiOverlay(RenderSession &session) : session{session}
{}

RenderSession &GuiOverlay::GetSession()
{
    return session;
}

void GuiOverlay::setGuiOverlayUpdateCb(void (*callback)(void *))
{
    guiUpdateCb = callback;
}

bool GuiOverlay::needUpdate() const
{
    return false;
}

} // namespace cruelEngine::cruelRender