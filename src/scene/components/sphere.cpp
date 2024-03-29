// #include "sphere.hpp"

// namespace cruelEngine {
// namespace cruelScene {
// void SphereTask::draw(cruelRender::CommandBuffer &commandBuffer, int index) {
//   std::cout << "Draw Comands Sphere: size = " << size << std::endl;

//   vkCmdBindPipeline(commandBuffer.get_handle(),
//   VK_PIPELINE_BIND_POINT_GRAPHICS,
//                     pipeline->get_handle());

//   VkBuffer vertexBuffers[] = {vertexBuffer};
//   VkDeviceSize offsets[] = {0};
//   vkCmdBindVertexBuffers(commandBuffer.get_handle(), 0, 1, vertexBuffers,
//                          offsets);

//   vkCmdBindIndexBuffer(commandBuffer.get_handle(), indexBuffer, 0,
//                        VK_INDEX_TYPE_UINT16);
//   vkCmdBindDescriptorSets(commandBuffer.get_handle(),
//                           VK_PIPELINE_BIND_POINT_GRAPHICS,
//                           pipeline_layout->get_handle(), 0, 1,
//                           &descriptor_set[index]->get_handle(), 0, nullptr);
//   vkCmdSetLineWidth(commandBuffer.get_handle(), 2.0f);
//   vkCmdDrawIndexed(commandBuffer.get_handle(), size, 1, 0, 0, 0);
// }

// void SphereTask::render() {
//   static auto startTime = std::chrono::high_resolution_clock::now();

//   auto currentTime = std::chrono::high_resolution_clock::now();
//   float time = std::chrono::duration<float, std::chrono::seconds::period>(
//                    currentTime - startTime)
//                    .count();

//   //   std::cout << "time diff: " << time << std::endl;

//   cruelScene::Sphere::UniformBufferObject ubo{};

//   ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f),
//                           glm::vec3(0.0f, 1.0f, 0.0f));
//   ubo.view = glm::lookAt(
//       glm::normalize(glm::vec3(1.0f, -1.0f, -2.0f)) * glm::vec3(3.0),
//       glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
//   ubo.proj =
//       glm::perspective(glm::radians(45.0f),
//                        session.get_swapchain().get_extent().width /
//                            (float)session.get_swapchain().get_extent().height,
//                        0.1f, 10.0f);
//   ubo.proj[1][1] *= -1;

//   uniform_buffer.update(&ubo);
// }

// Sphere::Sphere(cruelRender::RenderContext &render_context, u32 session_idx,
//                std::string name)
//     : Object(render_context, session_idx, name) {
//   create_buffer();

//   type = SCENE_OBJ_TYPE_SPHERE;
//   std::unique_ptr<cruelRender::RenderTask> new_task;

//   new_task = std::make_unique<SphereTask>(
//       render_context.get_session(session_idx),
//       render_context.get_session(session_idx).get_render_pass(),
//       buffer->get_handle(), indexbuffer->get_handle(), mesh.triangles.size(),
//       *uniform_buffer, descriptor_set);

//   task = new_task.get();
//   render_context.get_session(session_idx).add_new_task(std::move(new_task));
//   new_task.reset();

//   shaders.emplace_back(render_context.get_device(),
//                        "/home/yiwen/program/cruelworld/engine/data/shaders/"
//                        "wireframe/frag.spv",
//                        "main", VK_SHADER_STAGE_FRAGMENT_BIT);

//   shaders.emplace_back(render_context.get_device(),
//                        "/home/yiwen/program/cruelworld/engine/data/shaders/"
//                        "wireframe/vert.spv",
//                        "main", VK_SHADER_STAGE_VERTEX_BIT);

//   std::cout << "[" << name << "] Shader Created." << std::endl;
//   VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
//   pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
//   pipelineLayoutInfo.setLayoutCount = 1; // Optional
//   pipelineLayoutInfo.pSetLayouts =
//       &descriptor_set_layout->get_handle();         // Optional
//   pipelineLayoutInfo.pushConstantRangeCount = 0;    // Optional
//   pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

//   task->prepare_pipeline_layout(shaders, pipelineLayoutInfo);
//   std::cout << "[" << name << "] PipelineLayout Created." << std::endl;

//   cruelRender::PipelineStatus pipeline_state{};
//   cruelRender::PipelineStatus::VertexInputState vertex_input_state;

//   vertex_input_state.attributes.push_back(
//       Vertex::getAttributeDescriptions()[0]);
//   vertex_input_state.attributes.push_back(
//       Vertex::getAttributeDescriptions()[1]);
//   VkVertexInputAttributeDescription attributeDescriptions;

//   vertex_input_state.bindings.push_back(Vertex::getBindingDescription());

//   cruelRender::PipelineStatus::InputAssemblyState input_assembly_state;
//   cruelRender::PipelineStatus::RasterizationState rasterization_state = {
//       VK_FALSE,
//       VK_FALSE,
//       VK_POLYGON_MODE_LINE,
//       VK_CULL_MODE_BACK_BIT,
//       VK_FRONT_FACE_COUNTER_CLOCKWISE,
//       VK_FALSE};
//   cruelRender::PipelineStatus::ViewportState viewport_state;
//   cruelRender::PipelineStatus::MultisampleState multisample_state = {
//       VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.0f,
//       VK_NULL_HANDLE,        VK_FALSE, VK_FALSE};

//   cruelRender::PipelineStatus::DepthStencilState depth_stencil_state;
//   cruelRender::PipelineStatus::ColorBlendState color_blend_state;
//   cruelRender::PipelineStatus::ColorBlendAttachmentState attach = {
//       VK_FALSE,
//       VK_BLEND_FACTOR_ONE,
//       VK_BLEND_FACTOR_ZERO,
//       VK_BLEND_OP_ADD,
//       VK_BLEND_FACTOR_ONE,
//       VK_BLEND_FACTOR_ZERO,
//       VK_BLEND_OP_ADD,
//       VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
//           VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT};
//   color_blend_state.attachments.push_back(attach);

//   cruelRender::PipelineStatus::DynamicState dynamic_state;
//   pipeline_state.set_vertex_input_state(vertex_input_state);
//   pipeline_state.set_input_assembly_state(input_assembly_state);
//   pipeline_state.set_rasterization_state(rasterization_state);
//   pipeline_state.set_viewport_state(viewport_state);
//   pipeline_state.set_multisample_state(multisample_state);
//   pipeline_state.set_depth_stencil_state(depth_stencil_state);
//   pipeline_state.set_color_blend_state(color_blend_state);
//   pipeline_state.set_dynamic_state(dynamic_state);

//   pipeline_state.set_subpass_index(0);

//   task->prepare_pipeline(VK_NULL_HANDLE, pipeline_state);
//   std::cout << "[" << name << "] Pipeline Created." << std::endl;
// }

// Sphere::~Sphere() {
//   buffer.reset();
//   indexbuffer.reset();
//   uniform_buffer.reset();
//   descriptor_set.clear();
//   descriptor_set_layout.reset();
//   descriptor_pool.reset();
//   mesh.clear();
// }

// void Sphere::prepare() {}

// void Sphere::create_buffer() {

//   Icosahedron(mesh);

//   buffer = std::make_unique<cruelRender::Buffer>(
//       render_context.get_device(), sizeof(Vertex) * mesh.vertices.size(),
//       VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
//       VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
//   buffer->load((void *)mesh.vertices.data());

//   indexbuffer = std::make_unique<cruelRender::Buffer>(
//       render_context.get_device(), sizeof(uint16_t) * mesh.triangles.size(),
//       VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
//   indexbuffer->load((void *)mesh.triangles.data());

//   uniform_buffer = std::make_unique<cruelRender::UniformBuffer>(
//       render_context.get_device(), sizeof(UniformBufferObject));

//   int pool_size = render_context.get_session(session_idx)
//                       .get_swapchain()
//                       .get_images()
//                       .size();

//   VkDescriptorPoolSize poolSize{};
//   poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//   poolSize.descriptorCount = pool_size;

//   descriptor_pool = std::make_unique<cruelRender::DescriptorPool>(
//       render_context.get_device(), poolSize);

//   VkDescriptorSetLayoutBinding uboLayoutBinding{};
//   uboLayoutBinding.binding = 0;
//   uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
//   uboLayoutBinding.descriptorCount = 1;
//   uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
//   uboLayoutBinding.pImmutableSamplers = nullptr; // Optional
//   std::vector<VkDescriptorSetLayoutBinding> bindings = {};
//   bindings.push_back(uboLayoutBinding);

//   descriptor_set_layout = std::make_unique<cruelRender::DescriptorSetLayout>(
//       render_context.get_device(), bindings);
//   for (size_t i = 0; i < pool_size; i++) {
//     descriptor_set.push_back(std::make_unique<cruelRender::DescriptorSet>(
//         render_context.get_device(), *descriptor_set_layout,
//         *descriptor_pool));
//   }

//   VkDescriptorBufferInfo bufferInfo{};
//   bufferInfo.buffer = uniform_buffer->get_handle();
//   bufferInfo.offset = 0;
//   bufferInfo.range = sizeof(UniformBufferObject);
//   for (auto &a : descriptor_set) {
//     a->update(bufferInfo);
//   }
//   //   descriptor_set->update(bufferInfo);
// }

// void Icosahedron(Mesht &mesh) {
//   const double t = (1.0 + std::sqrt(5.0)) / 2.0;
//   // Vertices
//   //   mesh.vertices.emplace_back(glm::normalize(glm::vec3(-1.0, t, 0.0)));
//   //   mesh.vertices.emplace_back(glm::normalize(glm::vec3(1.0, t, 0.0)));
//   //   mesh.vertices.emplace_back(glm::normalize(glm::vec3(-1.0, -t, 0.0)));
//   //   mesh.vertices.emplace_back(glm::normalize(glm::vec3(1.0, -t, 0.0)));
//   //   mesh.vertices.emplace_back(glm::normalize(glm::vec3(0.0, -1.0, t)));
//   //   mesh.vertices.emplace_back(glm::normalize(glm::vec3(0.0, 1.0, t)));
//   //   mesh.vertices.emplace_back(glm::normalize(glm::vec3(0.0, -1.0, -t)));
//   //   mesh.vertices.emplace_back(glm::normalize(glm::vec3(0.0, 1.0, -t)));
//   //   mesh.vertices.emplace_back(glm::normalize(glm::vec3(t, 0.0, -1.0)));
//   //   mesh.vertices.emplace_back(glm::normalize(glm::vec3(t, 0.0, 1.0)));
//   //   mesh.vertices.emplace_back(glm::normalize(glm::vec3(-t, 0.0, -1.0)));
//   //   mesh.vertices.emplace_back(glm::normalize(glm::vec3(-t, 0.0, 1.0)));
//   //   // Faces
//   //   mesh.addTriangle(0, 11, 5);
//   //   mesh.addTriangle(0, 5, 1);
//   //   mesh.addTriangle(0, 1, 7);
//   //   mesh.addTriangle(0, 7, 10);
//   //   mesh.addTriangle(0, 10, 11);
//   //   mesh.addTriangle(1, 5, 9);
//   //   mesh.addTriangle(5, 11, 4);
//   //   mesh.addTriangle(11, 10, 2);
//   //   mesh.addTriangle(10, 7, 6);
//   //   mesh.addTriangle(7, 1, 8);
//   //   mesh.addTriangle(3, 9, 4);
//   //   mesh.addTriangle(3, 4, 2);
//   //   mesh.addTriangle(3, 2, 6);
//   //   mesh.addTriangle(3, 6, 8);
//   //   mesh.addTriangle(3, 8, 9);
//   //   mesh.addTriangle(4, 9, 5);
//   //   mesh.addTriangle(2, 4, 11);
//   //   mesh.addTriangle(6, 2, 10);
//   //   mesh.addTriangle(8, 6, 7);
//   //   mesh.addTriangle(9, 8, 1);

//   float a = 0.5;

//   mesh.vertices.emplace_back(
//       (Vertex){glm::normalize(glm::vec3(a, a, a)), glm::vec3(a, a, -a)});
//   mesh.vertices.emplace_back(
//       (Vertex){glm::normalize(glm::vec3(a, -a, a)), glm::vec3(a, -a, -a)});
//   mesh.vertices.emplace_back(
//       (Vertex){glm::normalize(glm::vec3(-a, -a, a)), glm::vec3(-a, -a, -a)});
//   mesh.vertices.emplace_back(
//       (Vertex){glm::normalize(glm::vec3(-a, a, a)), glm::vec3(-a, a, -a)});

//   mesh.vertices.emplace_back(
//       (Vertex){glm::normalize(glm::vec3(a, a, -a)), glm::vec3(a, a, a)});
//   mesh.vertices.emplace_back(
//       (Vertex){glm::normalize(glm::vec3(a, -a, -a)), glm::vec3(a, -a, a)});
//   mesh.vertices.emplace_back(
//       (Vertex){glm::normalize(glm::vec3(-a, -a, -a)), glm::vec3(-a, -a, a)});
//   mesh.vertices.emplace_back(
//       (Vertex){glm::normalize(glm::vec3(-a, a, -a)), glm::vec3(-a, a, a)});

//   mesh.addTriangle(0, 1, 5);
//   mesh.addTriangle(0, 5, 4);

//   mesh.addTriangle(0, 2, 1);
//   mesh.addTriangle(0, 3, 2);

//   mesh.addTriangle(1, 2, 5);
//   mesh.addTriangle(2, 6, 5);

//   mesh.addTriangle(6, 2, 3);
//   mesh.addTriangle(3, 7, 6);

//   mesh.addTriangle(4, 5, 6);
//   mesh.addTriangle(6, 7, 4);

//   mesh.addTriangle(0, 4, 3);
//   mesh.addTriangle(4, 7, 3);
// }
// } // namespace cruelScene
// } // namespace cruelEngine