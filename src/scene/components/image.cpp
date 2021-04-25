#include "image.hpp"

namespace cruelEngine::cruelScene
{
Image::Image(const std::string &name, std::vector<u8> &&data, std::vector<Mipmap> &&mipmap) :
    Component(name)
{}
std::type_index Image::GetType()
{
    return typeid(Image);
}
} // namespace cruelEngine::cruelScene