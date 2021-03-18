#include "mesh.hpp"

namespace cruelEngine::cruelScene {

VkVertexInputBindingDescription Vertex::getBindingDescription() {
  VkVertexInputBindingDescription bindingDescription{};
  bindingDescription.binding = 0;
  bindingDescription.stride = sizeof(Vertex);
  bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
  return bindingDescription;
}

std::array<VkVertexInputAttributeDescription, 2>
Vertex::getAttributeDescriptions() {
  std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
  attributeDescriptions[0].binding = 0;
  attributeDescriptions[0].location = 0;
  attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[0].offset = offsetof(Vertex, pos);
  attributeDescriptions[1].binding = 0;
  attributeDescriptions[1].location = 1;
  attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
  attributeDescriptions[1].offset = offsetof(Vertex, color);
  return attributeDescriptions;
}

Mesh::Mesh() {}

Mesh::~Mesh() { clear(); }

void Mesh::clear() {
  vertices.clear();
  triangles.clear();
}

void Mesh::addTriangle(u16 a, u16 b, u16 c) {
  triangles.emplace_back(a);
  triangles.emplace_back(b);
  triangles.emplace_back(c);
}

void Mesh::addVertex(Vertex vertex) { vertices.emplace_back(vertex); }

} // namespace cruelEngine::cruelScene