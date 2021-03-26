#pragma once
#include "../component.hpp"

namespace cruelEngine::cruelScene
{
enum class LightType
{
    LIGHT_TYPE_DIRECTIONAL = 0,
    LIGHT_TYPE_POINT,
    LIGHT_TYPE_SPOT,
};

/**
 * \brief Base class of light.
 */
class Light : public Component
{
public:
};
} // namespace cruelEngine::cruelScene