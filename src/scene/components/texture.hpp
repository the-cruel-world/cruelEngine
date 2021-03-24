#pragma once
#include "../component.hpp"

namespace cruelEngine::cruelScene {

class Texture : Component {
public:
  Texture(const std::string &name);

  virtual ~Texture() = default;

private:
};
} // namespace cruelEngine::cruelScene