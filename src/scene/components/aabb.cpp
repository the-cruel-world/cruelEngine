#include "aabb.hpp"

namespace cruelEngine::cruelScene
{
AABB::AABB()
{
    Reset();
}

AABB::AABB(const glm::vec3 &min, const glm::vec3 &max) : min{min}, max{max}
{}

void AABB::Reset()
{
    min = std::numeric_limits<glm::vec3>::max();

    max = std::numeric_limits<glm::vec3>::min();
}

} // namespace cruelEngine::cruelScene