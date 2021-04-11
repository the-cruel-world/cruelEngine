#pragma once
#include "sgcommon.h"

namespace cruelEngine::cruelScene
{
class Component
{
public:
    Component() = default;

    Component(const std::string &name);

    Component(Component &&other) = default;

    virtual ~Component() = default;

    const std::string &GetName() const
    {
        return name;
    }

    virtual std::type_index GetType() = 0;

protected:
    std::string name;
};
} // namespace cruelEngine::cruelScene