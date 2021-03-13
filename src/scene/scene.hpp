#pragma once
#include "../common.h"
#include "node.hpp"
#include "object.hpp"

namespace cruelEngine::cruelScene {

class Object;

class Scene {
public:
  Scene();
  virtual ~Scene();

  void addObject(std::unique_ptr<Object> &obj);

  void rmObject(std::string name);

  const std::vector<std::unique_ptr<Object>> &get_objs() const {
    return sceneObjects;
  }

  void update();

  void render();

protected:
  std::vector<std::unique_ptr<Object>> sceneObjects{};

  /**
   * All nodes in this scene.
   */
  std::vector<std::unique_ptr<Node>> nodes;
  void set_root_node(Node &node) { root = &node; }
  Node &get_root_node() { return *root; }
  /**
   * pointer to the root node.
   */
  Node *root = nullptr;
  Node *find_node(const std::string &name);
};
} // namespace cruelEngine::cruelScene