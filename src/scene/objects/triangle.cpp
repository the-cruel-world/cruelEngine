#include "triangle.hpp"

namespace cruelEngine {
namespace cruelScene {
void TriangleTask::draw(cruelRender::CommandBuffer &commandBuffer) {
  // std::cout << "Draw Comands Triangle." << std::endl;

  vkCmdBindPipeline(commandBuffer.get_handle(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                    pipeline->get_handle());

  VkBuffer vertexBuffers[] = {vertexBuffer};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer.get_handle(), 0, 1, vertexBuffers,
                         offsets);

  vkCmdBindIndexBuffer(commandBuffer.get_handle(), indexBuffer, 0,
                       VK_INDEX_TYPE_UINT16);

  vkCmdDrawIndexed(commandBuffer.get_handle(), 6, 1, 0, 0, 0);
}

Triangle::Triangle(cruelRender::RenderContext &render_context, u32 session_idx,
                   std::string name)
    : Object(render_context, session_idx, name) {
  createVertexBuffer();

  type = SCENE_OBJ_TYPE_TRIANGLE;
  std::unique_ptr<cruelRender::RenderTask> new_task;

  new_task = std::make_unique<TriangleTask>(
      render_context.get_session(session_idx),
      render_context.get_session(session_idx).get_render_pass(),
      buffer->get_handle(), indexbuffer->get_handle());

  task = new_task.get();
  render_context.get_session(session_idx).add_new_task(std::move(new_task));
  new_task.reset();

  shaders.emplace_back(
      render_context.get_device(),
      "/home/yiwen/program/cruelworld/engine/data/shaders/triangle/frag.spv",
      "main", VK_SHADER_STAGE_FRAGMENT_BIT);

  shaders.emplace_back(
      render_context.get_device(),
      "/home/yiwen/program/cruelworld/engine/data/shaders/triangle/vert.spv",
      "main", VK_SHADER_STAGE_VERTEX_BIT);

  std::cout << "[" << name << "] Shader Created." << std::endl;

  task->prepare_pipline_layout(shaders);
  std::cout << "[" << name << "] PipelineLayout Created." << std::endl;

  cruelRender::PipelineStatus pipeline_state{};
  cruelRender::PipelineStatus::VertexInputState vertex_input_state;

  vertex_input_state.attributes.push_back(
      Vertex::getAttributeDescriptions()[0]);
  vertex_input_state.attributes.push_back(
      Vertex::getAttributeDescriptions()[1]);
  vertex_input_state.bindings.push_back(Vertex::getBindingDescription());

  cruelRender::PipelineStatus::InputAssemblyState input_assembly_state;
  cruelRender::PipelineStatus::RasterizationState rasterization_state = {
      VK_FALSE,
      VK_FALSE,
      VK_POLYGON_MODE_FILL,
      VK_CULL_MODE_BACK_BIT,
      VK_FRONT_FACE_CLOCKWISE,
      VK_FALSE};
  cruelRender::PipelineStatus::ViewportState viewport_state;
  cruelRender::PipelineStatus::MultisampleState multisample_state = {
      VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.0f,
      VK_NULL_HANDLE,        VK_FALSE, VK_FALSE};

  cruelRender::PipelineStatus::DepthStencilState depth_stencil_state;
  cruelRender::PipelineStatus::ColorBlendState color_blend_state;
  cruelRender::PipelineStatus::ColorBlendAttachmentState attach = {
      VK_FALSE,
      VK_BLEND_FACTOR_ONE,
      VK_BLEND_FACTOR_ZERO,
      VK_BLEND_OP_ADD,
      VK_BLEND_FACTOR_ONE,
      VK_BLEND_FACTOR_ZERO,
      VK_BLEND_OP_ADD,
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
          VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT};
  color_blend_state.attachments.push_back(attach);

  cruelRender::PipelineStatus::DynamicState dynamic_state;
  pipeline_state.set_vertex_input_state(vertex_input_state);
  pipeline_state.set_input_assembly_state(input_assembly_state);
  pipeline_state.set_rasterization_state(rasterization_state);
  pipeline_state.set_viewport_state(viewport_state);
  pipeline_state.set_multisample_state(multisample_state);
  pipeline_state.set_depth_stencil_state(depth_stencil_state);
  pipeline_state.set_color_blend_state(color_blend_state);
  pipeline_state.set_dynamic_state(dynamic_state);

  pipeline_state.set_subpass_index(0);

  task->prepare_pipeline(VK_NULL_HANDLE, pipeline_state);
  std::cout << "[" << name << "] Pipeline Created." << std::endl;
}

Triangle::~Triangle() {
  buffer.reset();
  indexbuffer.reset();
}

void Triangle::prepare() {}

void Triangle::createVertexBuffer() {
  buffer = std::make_unique<cruelRender::Buffer>(
      render_context.get_device(), sizeof(vertices[0]) * vertices.size(),
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  buffer->load((void *)vertices.data());
  indexbuffer = std::make_unique<cruelRender::Buffer>(
      render_context.get_device(), sizeof(indices[0]) * indices.size(),
      VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  indexbuffer->load((void *)indices.data());
}
} // namespace cruelScene
} // namespace cruelEngine