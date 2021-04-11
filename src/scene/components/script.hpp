#pragma once
#include "scene/component.hpp"

namespace cruelEngine::cruelScene
{
class Script : public Component
{
public:
    Script(const std::string &name);

    std::type_index GetType() override;

private:
};

} // namespace cruelEngine::cruelScene