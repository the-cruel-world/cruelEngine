#include "sphere.hpp"

namespace cruelEngine::cruelScene
{
Sphere::Sphere() : Sphere(true)
{}

Sphere::Sphere(bool face_out) : Mesh()
{
    float a = 0.5;
    addVertex((Vertex){glm::normalize(glm::vec3(a, a, a)), glm::vec3(a, a, -a)});
    addVertex((Vertex){glm::normalize(glm::vec3(a, -a, a)), glm::vec3(a, -a, -a)});
    addVertex((Vertex){glm::normalize(glm::vec3(-a, -a, a)), glm::vec3(-a, -a, -a)});
    addVertex((Vertex){glm::normalize(glm::vec3(-a, a, a)), glm::vec3(-a, a, -a)});

    addVertex((Vertex){glm::normalize(glm::vec3(a, a, -a)), glm::vec3(a, a, a)});
    addVertex((Vertex){glm::normalize(glm::vec3(a, -a, -a)), glm::vec3(a, -a, a)});
    addVertex((Vertex){glm::normalize(glm::vec3(-a, -a, -a)), glm::vec3(-a, -a, a)});
    addVertex((Vertex){glm::normalize(glm::vec3(-a, a, -a)), glm::vec3(-a, a, a)});

    addTriangle(0, 1, 5);
    addTriangle(0, 5, 4);

    addTriangle(0, 2, 1);
    addTriangle(0, 3, 2);

    addTriangle(1, 2, 5);
    addTriangle(2, 6, 5);

    addTriangle(6, 2, 3);
    addTriangle(3, 7, 6);

    addTriangle(4, 5, 6);
    addTriangle(6, 7, 4);

    addTriangle(0, 4, 3);
    addTriangle(4, 7, 3);
}

} // namespace cruelEngine::cruelScene