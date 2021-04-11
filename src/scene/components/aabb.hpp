#pragma once
#include "scene/component.hpp"
#include "scene/object.hpp"

namespace cruelEngine::cruelScene
{
/**
 * \brief AABB bound box for bvh acceleration structure.
 */
class AABB : public Component
{
public:
    AABB();

    AABB(const glm::vec3 &min, const glm::vec3 &max);

    virtual ~AABB() = default;

    std::type_index GetType() override;

    //   void update(const glm::vec3 &point);

    glm::vec3 GetScale() const
    {
        return (max - min);
    }

    glm::vec3 GetCenter() const
    {
        return (max + min) * 0.5f;
    }

    glm::vec3 GetMax() const
    {
        return max;
    }

    glm::vec3 GetMin() const
    {
        return min;
    }

    void Reset();

private:
    glm::vec3 min;

    glm::vec3 max;
};
} // namespace cruelEngine::cruelScene