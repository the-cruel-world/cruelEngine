#include "../../scene/mesh.hpp"
#include "../../scene/scene_objects/skybox.hpp"
#include "render_tasks.hpp"

namespace cruelEngine::cruelRender {

GeomTask::GeomTask(RenderSession &session,
                   std::shared_ptr<cruelScene::Object> object)
    : RenderTask(session, object) {
  prepare();
};

void GeomTask::draw(cruelRender::CommandBuffer &commandBuffer, int index) {
  vkCmdBindPipeline(commandBuffer.get_handle(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                    pipeline->get_handle());

  VkBuffer vertexBuffers[] = {vertex_buffer->get_handle()};
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(commandBuffer.get_handle(), 0, 1, vertexBuffers,
                         offsets);

  vkCmdBindIndexBuffer(commandBuffer.get_handle(), index_buffer->get_handle(),
                       0, VK_INDEX_TYPE_UINT16);
  vkCmdBindDescriptorSets(commandBuffer.get_handle(),
                          VK_PIPELINE_BIND_POINT_GRAPHICS,
                          pipeline_layout->get_handle(), 0, 1,
                          &descriptor_set[index]->get_handle(), 0, nullptr);
  vkCmdSetLineWidth(commandBuffer.get_handle(), 2.0f);
  vkCmdDrawIndexed(commandBuffer.get_handle(),
                   object->get_mesh().get_triangles().size(), 1, 0, 0, 0);
}

void GeomTask::render() {
  static auto startTime = std::chrono::high_resolution_clock::now();

  auto currentTime = std::chrono::high_resolution_clock::now();
  float time = std::chrono::duration<float, std::chrono::seconds::period>(
                   currentTime - startTime)
                   .count();

  UniformBufferObject ubo{};

  ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),
                          glm::vec3(0.0f, 1.0f, 0.0f));
  ubo.view = glm::lookAt(
      glm::normalize(glm::vec3(1.0f, -1.0f, -2.0f)) * glm::vec3(3.0),
      glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
  ubo.proj =
      glm::perspective(glm::radians(45.0f),
                       session.get_swapchain().get_extent().width /
                           (float)session.get_swapchain().get_extent().height,
                       0.1f, 10.0f);
  ubo.proj[1][1] *= -1;

  uniform_buffer->update(&ubo);
}

void GeomTask::prepare() {
  prepare_assets();

  std::vector<ShaderModule> shaders{};
  shaders.emplace_back(session.get_device(),
                       "/home/yiwen/program/cruelworld/engine/data/shaders/"
                       "wireframe/frag.spv",
                       "main", VK_SHADER_STAGE_FRAGMENT_BIT);

  shaders.emplace_back(session.get_device(),
                       "/home/yiwen/program/cruelworld/engine/data/shaders/"
                       "wireframe/vert.spv",
                       "main", VK_SHADER_STAGE_VERTEX_BIT);

  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 1; // Optional
  pipelineLayoutInfo.pSetLayouts =
      &descriptor_set_layout->get_handle();         // Optional
  pipelineLayoutInfo.pushConstantRangeCount = 0;    // Optional
  pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

  prepare_pipeline_layout(shaders, pipelineLayoutInfo);

  PipelineStatus pipeline_state{};
  PipelineStatus::VertexInputState vertex_input_state;

  vertex_input_state.attributes.push_back(
      cruelScene::Vertex::getAttributeDescriptions()[0]);
  vertex_input_state.attributes.push_back(
      cruelScene::Vertex::getAttributeDescriptions()[1]);
  VkVertexInputAttributeDescription attributeDescriptions;

  vertex_input_state.bindings.push_back(
      cruelScene::Vertex::getBindingDescription());

  PipelineStatus::InputAssemblyState input_assembly_state;
  PipelineStatus::RasterizationState rasterization_state = {
      VK_FALSE,
      VK_FALSE,
      VK_POLYGON_MODE_LINE,
      VK_CULL_MODE_BACK_BIT,
      VK_FRONT_FACE_COUNTER_CLOCKWISE,
      VK_FALSE};
  PipelineStatus::ViewportState viewport_state;
  PipelineStatus::MultisampleState multisample_state = {
      VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.0f,
      VK_NULL_HANDLE,        VK_FALSE, VK_FALSE};

  PipelineStatus::DepthStencilState depth_stencil_state;
  PipelineStatus::ColorBlendState color_blend_state;
  PipelineStatus::ColorBlendAttachmentState attach = {
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

  PipelineStatus::DynamicState dynamic_state;
  pipeline_state.set_vertex_input_state(vertex_input_state);
  pipeline_state.set_input_assembly_state(input_assembly_state);
  pipeline_state.set_rasterization_state(rasterization_state);
  pipeline_state.set_viewport_state(viewport_state);
  pipeline_state.set_multisample_state(multisample_state);
  pipeline_state.set_depth_stencil_state(depth_stencil_state);
  pipeline_state.set_color_blend_state(color_blend_state);
  pipeline_state.set_dynamic_state(dynamic_state);

  pipeline_state.set_subpass_index(0);

  prepare_pipeline(VK_NULL_HANDLE, pipeline_state);
}

void GeomTask::prepare_pipeline(VkPipelineCache pipeline_cache,
                                PipelineStatus &pipeline_state) {
  pipeline_state.set_pipeline_layout(*pipeline_layout);

  pipeline_state.set_render_pass(session.get_render_pass());

  // std::cout << "direct: " << pipeline_layout->get_handle() << std::endl;
  // std::cout << "indire: " <<
  // pipeline_state.get_pipeline_layout()->get_handle() << std::endl;
  pipeline = std::make_unique<GraphicsPipeline>(session.get_device(),
                                                pipeline_cache, pipeline_state);
}

void GeomTask::prepare_assets() {
  vertex_buffer = std::make_unique<cruelRender::Buffer>(
      session.get_device(),
      sizeof(cruelScene::Vertex) * object->get_mesh().get_vertices().size(),
      VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  vertex_buffer->load((void *)object->get_mesh().get_vertices().data());

  index_buffer = std::make_unique<cruelRender::Buffer>(
      session.get_device(),
      sizeof(uint16_t) * object->get_mesh().get_triangles().size(),
      VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  index_buffer->load((void *)object->get_mesh().get_triangles().data());

  uniform_buffer = std::make_unique<cruelRender::UniformBuffer>(
      session.get_device(), sizeof(UniformBufferObject));

  int pool_size = session.get_swapchain().get_images().size();

  VkDescriptorPoolSize poolSize{};
  poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  poolSize.descriptorCount = pool_size;

  descriptor_pool = std::make_unique<cruelRender::DescriptorPool>(
      session.get_device(), poolSize);

  VkDescriptorSetLayoutBinding uboLayoutBinding{};
  uboLayoutBinding.binding = 0;
  uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  uboLayoutBinding.descriptorCount = 1;
  uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
  uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
  std::vector<VkDescriptorSetLayoutBinding> bindings = {};
  bindings.push_back(uboLayoutBinding);

  descriptor_set_layout = std::make_unique<cruelRender::DescriptorSetLayout>(
      session.get_device(), bindings);
  for (size_t i = 0; i < pool_size; i++) {
    descriptor_set.push_back(std::make_unique<cruelRender::DescriptorSet>(
        session.get_device(), *descriptor_set_layout, *descriptor_pool));
  }

  VkDescriptorBufferInfo bufferInfo{};
  bufferInfo.buffer = uniform_buffer->get_handle();
  bufferInfo.offset = 0;
  bufferInfo.range = sizeof(UniformBufferObject);
  for (auto &a : descriptor_set) {
    a->update(bufferInfo);
  }
}

} // namespace cruelEngine::cruelRender