// #pragma once
// #include "../../render/render_header.h"
// #include "../../render/vkcommon.h"
// #include "../object.hpp"

// namespace cruelEngine {
// namespace cruelScene {
// class TriangleTask : public cruelRender::RenderTask {
// public:
//   TriangleTask(
//       cruelRender::RenderSession &session, cruelRender::RenderPass
//       &render_pass, const VkBuffer &vertexBuffer, const VkBuffer
//       &indexBuffer, cruelRender::UniformBuffer &uniform_buffer,
//       std::vector<std::unique_ptr<cruelRender::DescriptorSet>>
//       &descriptor_set) : cruelRender::RenderTask(session, render_pass),
//         vertexBuffer{vertexBuffer}, indexBuffer{indexBuffer},
//         uniform_buffer{uniform_buffer}, descriptor_set{descriptor_set} {}
//   void draw(cruelRender::CommandBuffer &commandBuffer, int index);
//   void render();

// private:
//   const VkBuffer &vertexBuffer;
//   const VkBuffer &indexBuffer;
//   cruelRender::UniformBuffer &uniform_buffer;
//   std::vector<std::unique_ptr<cruelRender::DescriptorSet>> &descriptor_set;
// };

// class Triangle : public Object {
// public:
//   struct Vertex {
//     glm::vec3 pos;
//     glm::vec3 color;

//     static VkVertexInputBindingDescription getBindingDescription() {
//       VkVertexInputBindingDescription bindingDescription{};
//       bindingDescription.binding = 0;
//       bindingDescription.stride = sizeof(Vertex);
//       bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
//       return bindingDescription;
//     }

//     static std::array<VkVertexInputAttributeDescription, 2>
//     getAttributeDescriptions() {
//       std::array<VkVertexInputAttributeDescription, 2>
//       attributeDescriptions{}; attributeDescriptions[0].binding = 0;
//       attributeDescriptions[0].location = 0;
//       attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
//       attributeDescriptions[0].offset = offsetof(Vertex, pos);
//       attributeDescriptions[1].binding = 0;
//       attributeDescriptions[1].location = 1;
//       attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
//       attributeDescriptions[1].offset = offsetof(Vertex, color);
//       return attributeDescriptions;
//     }
//   };

//   struct UniformBufferObject {
//     glm::mat4 model;
//     glm::mat4 view;
//     glm::mat4 proj;
//   };

//   Triangle(cruelRender::RenderContext &render_context, u32 session_idx,
//            std::string name);
//   ~Triangle();

//   void prepare();

//   void create_buffer();

// private:
//   const std::vector<Vertex> vertices = {
//       {{-.5f, -.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
//       {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
//       {{-.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
//       {{0.5f, -.5f, 0.0f}, {1.0f, 1.0f, 0.0f}},
//   };
//   const std::vector<uint16_t> indices = {0, 2, 1, 1, 3, 0};
//   std::unique_ptr<cruelRender::Buffer> buffer;
//   std::unique_ptr<cruelRender::Buffer> indexbuffer;
//   std::unique_ptr<cruelRender::UniformBuffer> uniform_buffer;
//   std::vector<std::unique_ptr<cruelRender::DescriptorSet>> descriptor_set;
//   std::unique_ptr<cruelRender::DescriptorSetLayout> descriptor_set_layout;
//   std::unique_ptr<cruelRender::DescriptorPool> descriptor_pool;
// };
// } // namespace cruelScene
// } // namespace cruelEngine