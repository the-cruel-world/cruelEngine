#pragma once
#include "vkcommon.h"
#include "vulkan/command_buffer.hpp"
#include "vulkan/command_pool.hpp"
#include "vulkan/descriptor.hpp"
#include "vulkan/image.hpp"
#include "vulkan/image_view.hpp"
#include "vulkan/pipeline.hpp"
#include "vulkan/queue.hpp"
#include "vulkan/sampler.hpp"

namespace cruelEngine::cruelRender
{
class LogicalDevice;
class RenderSession;

class GuiOverlay
{
public:
    GuiOverlay(RenderSession &session);

    virtual ~GuiOverlay() = default;

    RenderSession &getSession();

    void setGuiOverlayUpdateCb(void (*callback)(void *));

    virtual void Draw(CommandBuffer &commandBuffer) = 0;

    virtual bool needUpdate() const = 0;

    virtual void update() = 0;

    virtual void renderFrame() = 0;

    virtual void resize(uint32_t width, uint32_t height) = 0;

protected:
    RenderSession &session;

    void (*guiUpdateCb)(void *) = nullptr;
};

} // namespace cruelEngine::cruelRender