#pragma once
#include "../common.h"
#include "../render/vkcommon.h"
#include "../types.h"

namespace cruelEngine::cruelScene {

struct Texture {
  glm::vec3 pos;
  glm::vec3 normal;
  //   glm::vec2 uv;
};

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;
  // glm::vec3 normal;
  // glm::vec2 uv;

  Vertex(const glm::vec3 _pos, const glm::vec3 _color)
      : pos(_pos), color(_color) {}

  static std::array<VkVertexInputAttributeDescription, 2>
  getAttributeDescriptions();

  static VkVertexInputBindingDescription getBindingDescription();
};

class Mesh {
public:
  Mesh();

  virtual ~Mesh();

  std::vector<Vertex> get_vertices() { return vertices; }

  std::vector<u16> get_triangles() { return triangles; }

  void addTriangle(u16 a, u16 b, u16 c);

  void addVertex(Vertex vertex);

  void clear();

protected:
  std::vector<Vertex> vertices{};

  std::vector<u16> triangles{};
};
} // namespace cruelEngine::cruelScene