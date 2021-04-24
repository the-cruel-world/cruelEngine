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
enum VertexFormat
{
    VERTEX_FORMAT_UNDEFINED = 0,
    VERTEX_FORMAT_R32_FLOAT,
    VERTEX_FORMAT_R32G32_FLOAT,
    VERTEX_FORMAT_R32G32B32_FLOAT,
    VERTEX_FORMAT_R32G32B32A32_FLOAT,
};

typedef struct VertexAttribute
{
    VertexFormat format = VERTEX_FORMAT_UNDEFINED;
    u32          offset = 0;
    u32          stride = 0;
} VertexAttribute;

class Primitive : public Component
{
public:
    ~Primitive();

    std::type_index GetType() override;

    // Boundary box
    void        UpdateBounds(const glm::vec3 &new_min, const glm::vec3 &new_max);
    void        UpdateBounds(const AABB &bound);
    const AABB &GetBounds() const;

    // Material
    void            SetMaterial();
    const Material *GetMaterial();

    // Vertex attribute
    const std::vector<VertexAttribute> &GetVertexAttributes() const;
    void                                SetVertexAttributes(std::vector<VertexAttribute> &&);
    void                                SetVertexLength(const size_t new_length);
    size_t                              GetVertexLength();

    // The vertex data
    void        SetData(char *new_data, size_t size);
    const char *GetData() const;
    char *      GetDataMutant();
    void        SetDataSize(size_t new_size);
    size_t      GetDataSize();

private:
    AABB bounds;

    char *data{nullptr};

    char *vertices{nullptr};
    char *indices{nullptr};

    size_t size = 0;

    // vertex have different use.
    std::vector<VertexAttribute> vertexAttributes{};

    size_t length;

    const Material *material{nullptr};
};

} // namespace cruelEngine::cruelScene

#endif // CRUELENGINE_PRIMITIVE_H
