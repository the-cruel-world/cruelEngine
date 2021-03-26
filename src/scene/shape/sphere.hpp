#pragma once
#include "../mesh.hpp"

namespace cruelEngine::cruelScene
{
class Sphere : public Mesh
{
public:
    Sphere(bool face_out);

    Sphere();

private:
    bool face_out = true;
};
} // namespace cruelEngine::cruelScene