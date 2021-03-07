#pragma once
#include "../common.h"

namespace cruelEngine {
namespace cruelScene {
struct Mesh {
  Mesh() {}
  ~Mesh() {}
};

struct Texture {
  Texture() {}
  ~Texture() {}
};

struct Vertex {
  glm::vec3 pos;
  glm::vec3 color;

  Vertex(const glm::vec3 _pos, const glm::vec3 _color)
      : pos(_pos), color(_color) {}
};
} // namespace cruelScene
} // namespace cruelEngine