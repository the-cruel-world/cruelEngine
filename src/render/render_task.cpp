#include "render_task.hpp"

#include "render_session.hpp"
#include "vulkan/command_buffer.hpp"
#include "vulkan/pipeline.hpp"
#include "vulkan/queue.hpp"
#include "vulkan/render_pass.hpp"
#include "vulkan/shader.hpp"
#include "vulkan/swapchain.hpp"

namespace cruelEngine {
namespace cruelRender {
RenderTask::RenderTask(RenderSession &session, RenderPass &render_pass)
    : session{session}, render_pass{render_pass} {}

RenderTask::~RenderTask() {}

void RenderTask::prepare() {}

void RenderTask::draw(CommandBuffer &commandBuffer, int index) {
  // commandBuffer.bindPipeLine(pipeline->get_handle(),
  // VK_PIPELINE_BIND_POINT_GRAPHICS);
  vkCmdBindPipeline(commandBuffer.get_handle(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                    pipeline->get_handle());
  vkCmdDraw(commandBuffer.get_handle(), 3, 1, 0, 0);
}

void RenderTask::render() {}

void RenderTask::prepare_pipeline(VkPipelineCache pipeline_cache,
                                  PipelineStatus &pipeline_state) {
  pipeline_state.set_pipeline_layout(*pipeline_layout);

  pipeline_state.set_render_pass(render_pass);

  // std::cout << "direct: " << pipeline_layout->get_handle() << std::endl;
  // std::cout << "indire: " <<
  // pipeline_state.get_pipeline_layout()->get_handle() << std::endl;
  pipeline = std::make_unique<GraphicsPipeline>(session.get_device(),
                                                pipeline_cache, pipeline_state);
}

void RenderTask::prepare_pipline_layout(
    const std::vector<ShaderModule> &shaders) {
  pipeline_layout =
      std::make_unique<PipelineLayout>(session.get_device(), shaders);
  // std::cout << "direct: " << pipeline_layout->get_handle() << std::endl;
}

void RenderTask::prepare_pipline_layout(
    const std::vector<ShaderModule> &shaders,
    const VkPipelineLayoutCreateInfo &pipelineLayoutInfo) {
  pipeline_layout = std::make_unique<PipelineLayout>(
      session.get_device(), shaders, pipelineLayoutInfo);
}

} // namespace cruelRender
} // namespace cruelEngine