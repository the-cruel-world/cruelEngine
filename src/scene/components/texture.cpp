#include "texture.hpp"

namespace cruelEngine::cruelScene
{
Texture::Texture(const std::string &name) : Component(name)
{}
std::type_index Texture::GetType()
{
    return typeid(Texture);
}
} // namespace cruelEngine::cruelScene