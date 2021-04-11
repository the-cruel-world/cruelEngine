#include "transform.hpp"

namespace cruelEngine::cruelScene
{
void Transform::SetTranslation(const glm::vec3 &newTranslation)
{
    translation = newTranslation;
}

void Transform::SetRotation(const glm::quat &newRotation)
{
    rotation = newRotation;
}

void Transform::SetScale(const glm::vec3 &newScale)
{
    scale = newScale;
}

void Transform::SetWorldMatrix(const glm::mat4 &newWorldMatrix)
{
    worldMatrix = newWorldMatrix;
}
std::type_index Transform::GetType()
{
    return typeid(Transform);
}
} // namespace cruelEngine::cruelScene