
#pragma once
#include "../common.h"

namespace cruelEngine::cruelScene {

class Component {
public:
  Component() {}
  ~Component() {}

protected:
};

class Node {
public:
  Node() {}
  ~Node() {}

  void update();

protected:
  Node *parent;

  uint32_t index;

  std::vector<Node *> children;

  std::vector<std::shared_ptr<Component>> components;

  glm::mat3 scale;
  glm::mat3 translation;
  glm::quat rotation{};
};

} // namespace cruelEngine::cruelScene