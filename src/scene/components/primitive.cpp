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
}; // namespace cruelEngine::cruelScene