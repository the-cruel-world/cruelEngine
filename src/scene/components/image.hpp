#pragma once
#include "scene/component.hpp"

namespace cruelEngine::cruelScene
{
typedef struct Extent2D
{
    float width;
    float height;
} Extent2D;

struct Mipmap
{
    u32 level  = 0;
    u32 offset = 0;

    Extent2D extent;
};

/**
 * \brief Image class to load images from file.
 */
class Image : Component
{
public:
    Image(const std::string &name, std::vector<u8> &&data = {},
          std::vector<Mipmap> &&mipmap = {{}});

    std::type_index GetType() override;

private:
    std::vector<u8> data = {};

    u32 layers = 1;

    std::vector<Mipmap> mipmap{};
};
} // namespace cruelEngine::cruelScene