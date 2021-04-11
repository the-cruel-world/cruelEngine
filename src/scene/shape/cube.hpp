#pragma once
#include "scene/components/mesh.hpp"

namespace cruelEngine::cruelScene
{
class Cube : public Mesh
{
public:
    Cube(bool face_out);

    Cube();

private:
    bool face_out = true;
};
} // namespace cruelEngine::cruelScene