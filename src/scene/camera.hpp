#pragma once

#include "../common.h"
#include "object.hpp"

namespace cruelEngine::cruelScene {
  
class Camera : public Object {
public:
  Camera(const std::string &name);

  virtual ~Camera();

  virtual glm::mat4 get_projection() = 0;

  glm::mat4 get_view();

  float get_aspect_ratio() const;
  void set_aspect_ratio(float ratio);

  float get_fov() const;
  void set_fov(float fov);

  float get_far_plane() const;
  void set_far_plane(float zfar);

  float get_near_plane() const;
  void set_near_plane(float znear);

private:
  float aspect_ratio{1.0f};

  float fov{glm::radians(60.0f)};

  float far_plane{100.0};

  float near_plane{0.1f};
};
} // namespace cruelEngine::cruelScene