
#pragma once
#include "../common.h"

namespace cruelEngine::cruelScene {

class Node {
public:
  Node() {}
  ~Node() {}

  void update();

protected:
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

} // namespace cruelEngine::cruelScene