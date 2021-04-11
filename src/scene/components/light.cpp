#include "light.hpp"

namespace cruelEngine::cruelScene
{
std::type_index Light::GetType()
{
    return typeid(Light);
}
} // namespace cruelEngine::cruelScene