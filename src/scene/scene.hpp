#pragma once
#include "../common.h"
#include "../render/render_header.h"
#include "object.hpp"

namespace cruelEngine {
class RenderContext;
namespace cruelRender {
class RenderContext;
}

namespace cruelScene {

class Object;

class Node {
public:
  Node() {}
  ~Node() {}

  void update();

private:
  Node *parent;
  uint32_t index;
  std::vector<Node *> children;

  glm::mat3 scale;
  glm::mat3 translation;
  glm::quat rotation{};
};

class Component {
public:
  Component() {}
  ~Component() {}

protected:
};

class Scene {
public:
  Scene(cruelRender::RenderContext &render_context);
  virtual ~Scene();

  void addObject(std::string name, u32 session_idx, ObjectType type);

  void rmObject(std::string name);

  const std::vector<std::unique_ptr<Object>> &get_objs() const {
    return sceneObjects;
  }

  void update();

  void render();

protected:
  std::vector<std::unique_ptr<Object>> sceneObjects{};

  cruelRender::RenderContext &render_context;
};
} // namespace cruelScene
} // namespace cruelEngine