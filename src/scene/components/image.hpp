#pragma once
#include "scene/component.hpp"

namespace cruelEngine::cruelScene
{
/**
 * \brief Image class to load images from file.
 */
class Image : Component
{
public:
    Image(const std::string &name);


    std::type_index GetType() override;
private:
};
} // namespace cruelEngine::cruelScene