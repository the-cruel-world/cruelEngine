#pragma once
#include "scene/component.hpp"

namespace cruelEngine::cruelScene
{
class Transform : public Component
{
public:
    // Transform();

    virtual ~Transform() = default;

    void SetTranslation(const glm::vec3 &newTranslation);

    void SetRotation(const glm::quat &newRotation);

    void SetScale(const glm::vec3 &newScale);

    void SetWorldMatrix(const glm::mat4 &newWorldMatrix);

    const glm::vec3 &GetTranslation() const
    {
        return translation;
    }

    const glm::quat &GetRotation() const
    {
        return rotation;
    }

    const glm::vec3 &GetScale() const
    {
        return scale;
    }

    const glm::mat4 &GetWorldMatrix() const
    {
        return worldMatrix;
    }

private:
    glm::vec3 translation = glm::vec3(0.0f);

    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    glm::vec3 scale = glm::vec3(1.0f);

    glm::mat4 worldMatrix = glm::mat4(1.0f);

    void UpdateWorldTransform();
};
} // namespace cruelEngine::cruelScene