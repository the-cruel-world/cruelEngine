#pragma once

namespace cruelEngine::cruelScene {

/**
 * \brief AlphaMode defines different types of transparent.
 */
enum class AlphaMode {
  ALPHAMODE_OPAQUE = 0,
  ALPHAMODE_MASK,
  ALPHAMODE_BLEND,
};

/**
 * \brief Material defines the physical properties of objects.
 */
class Material {
public:
  Material();
  ~Material();

private:
  AlphaMode mode;
};
} // namespace cruelEngine::cruelScene