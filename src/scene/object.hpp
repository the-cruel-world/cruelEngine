#pragma once

#include "../common.h"
#include "../types.h"
#include "mesh.hpp"

namespace cruelEngine::cruelScene {

class Mesh;
struct Texture;
struct Vertex;

enum ObjectType {
  SCENE_OBJ_TYPE_GEOM = 0,
  SCENE_OBJ_TYPE_CUBE,
  SCENE_OBJ_TYPE_SPHERE,
};

class Object {
public:
  Object(std::string name);

  virtual ~Object();

  // virtual void prepare() = 0;

  // virtual void loadAsset(){};

  // virtual void unloadAsset(){};

  const std::string &get_name() const { return name; }

  const ObjectType get_type() const { return type; }

  // bool is_alive() const;

  // void show_name() const { std::cout << "Object : " << name << std::endl; }

protected:
  std::string name;
  // std::chrono::high_resolution_clock         lifetime;

  ObjectType type;

  // std::vector<Mesh> mesh;
  // std::vector<Texture> textures;
  // std::vector<Vertex> vertices;
};
} // namespace cruelEngine::cruelScene