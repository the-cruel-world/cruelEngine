#pragma once
#include "../common.h"

namespace cruelEngine::cruelScene {
class Component {
public:
  Component() = default;

  Component(const std::string &name);

  Component(Component &&other) = default;

  virtual ~Component() = default;

  const std::string &GetName() const { return name; }

protected:
  std::string name;
};
} // namespace cruelEngine::cruelScene