#include "image.hpp"

namespace cruelEngine::cruelScene
{
Image::Image(const std::string &name) : Component(name)
{}
std::type_index Image::GetType()
{
    return typeid(Image);
}
} // namespace cruelEngine::cruelScene