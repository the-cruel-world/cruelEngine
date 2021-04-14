//
// Created by yiwen on 4/11/21.
//

#ifndef CRUELENGINE_PRIMITIVE_H
#define CRUELENGINE_PRIMITIVE_H
#pragma once

#include "scene/component.hpp"
#include "scene/components/aabb.hpp"
#include "scene/components/material.hpp"

namespace cruelEngine::cruelScene
{
enum

    typedef struct VertexAttribute
{
    u32 stride = 0; u32 offset = 0;
}
VertexAttribute;

class Primitive : public Component
{
public:
    ~Primitive();

    std::type_index GetType() override;

    void UpdateBounds();

    const AABB &GetBounds() const;

    const std::vector<VertexAttribute> &GetVertexAttributes() const;

    void SetVertexAttributes(std::vector<VertexAttribute> &&);

    void SetData(char *new_data, size_t size);

    const char *GetData() const;

private:
    AABB bounds;

    char *data{nullptr};

    size_t size = 0;

    // vertex have different use.
    std::vector<VertexAttribute> vertexAttributes{};

    const Material *material{nullptr};
};

} // namespace cruelEngine::cruelScene

#endif // CRUELENGINE_PRIMITIVE_H
