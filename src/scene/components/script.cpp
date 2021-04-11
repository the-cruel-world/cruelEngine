#include "script.hpp"

namespace cruelEngine::cruelScene
{
Script::Script(const std::string &name) : Component{name}
{}
std::type_index Script::GetType()
{
    return typeid(Script);
}

} // namespace cruelEngine::cruelScene