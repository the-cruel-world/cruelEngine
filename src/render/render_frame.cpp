#include "render_frame.hpp"
#include "vulkan/command_buffer.hpp"
#include "vulkan/render_pass.hpp"

namespace cruelEngine::cruelRender
{
RenderFrame::RenderFrame(Image &image, ImageView &imageView, FrameBuffer &frameBuffer,
                         RenderPass &renderPass, CommandBuffer &commandBuffer) :
    image{std::move(image)},
    imageView{std::move(imageView)},
    frameBuffer{std::move(frameBuffer)},
    renderPass{renderPass},
    commandBuffer{commandBuffer}
{}

RenderFrame::~RenderFrame()
{}

void RecordBegin();

void RecordEnd();

void Submit();

void Present();

} // namespace cruelEngine::cruelRender