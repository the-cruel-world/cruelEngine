#pragma once
#include "scene/component.hpp"

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

    std::type_index GetType() override;

};
} // namespace cruelEngine::cruelScene