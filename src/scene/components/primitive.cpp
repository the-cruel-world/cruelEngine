//
// Created by yiwen on 4/11/21.
//

#include "primitive.hpp"

namespace cruelEngine::cruelScene
{
Primitive::~Primitive()
{
    if (data != nullptr)
    {
        delete[] data;
    }
}

std::type_index Primitive::GetType()
{
    return typeid(Primitive);
}

const std::vector<VertexAttribute> &Primitive::GetVertexAttributes() const
{
    return vertexAttributes;
}

void Primitive::SetVertexAttributes(std::vector<VertexAttribute> &&newVertexAttributes)
{
    vertexAttributes = std::move(newVertexAttributes);
}

void Primitive::SetData(char *new_data, size_t new_size)
{
    if (new_size * 5 < size || new_size > size)
    {
        size = new_size;
        delete[] data;
        data = new char[new_size];
    }
    else if (data == nullptr)
    {
        size = new_size;
        data = new char[new_size];
    }
    memcpy(data, new_data, new_size);
}

const char *Primitive::GetData() const
{
    return data;
}
char *Primitive::GetDataMutant()
{
    return data;
}
void Primitive::SetDataSize(size_t new_size)
{
    this->size = new_size;
}
void Primitive::UpdateBounds(const glm::vec3 &new_min, const glm::vec3 &new_max)
{
    bounds.update(new_min, new_max);
}
void Primitive::UpdateBounds(const AABB &bound)
{
    UpdateBounds(bound.GetMin(), bound.GetMax());
}
size_t Primitive::GetDataSize()
{
    return size;
}
void Primitive::SetVertexLength(const size_t new_length)
{
    length = new_length;
}
size_t Primitive::GetVertexLength()
{
    return length;
}
}; // namespace cruelEngine::cruelScene