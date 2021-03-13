#include "../mesh.hpp"
#include "../object.hpp"

namespace cruelEngine::cruelScene {

enum SkyBoxGeomType {
  SKY_BOX_GEOM_CUBE = 0,
  SKY_BOX_GEOM_SPHERE,
};

class SkyBox : public Object {
  SkyBox(std::string name) : Object(name) {
    type = SCENE_OBJ_TYPE_GEOM;

    mesh = std::make_unique<Mesh>();
    float a = 0.5;

    mesh->addVertex(
        (Vertex){glm::normalize(glm::vec3(a, a, a)), glm::vec3(a, a, -a)});
    mesh->addVertex(
        (Vertex){glm::normalize(glm::vec3(a, -a, a)), glm::vec3(a, -a, -a)});
    mesh->addVertex(
        (Vertex){glm::normalize(glm::vec3(-a, -a, a)), glm::vec3(-a, -a, -a)});
    mesh->addVertex(
        (Vertex){glm::normalize(glm::vec3(-a, a, a)), glm::vec3(-a, a, -a)});

    mesh->addVertex(
        (Vertex){glm::normalize(glm::vec3(a, a, -a)), glm::vec3(a, a, a)});
    mesh->addVertex(
        (Vertex){glm::normalize(glm::vec3(a, -a, -a)), glm::vec3(a, -a, a)});
    mesh->addVertex(
        (Vertex){glm::normalize(glm::vec3(-a, -a, -a)), glm::vec3(-a, -a, a)});
    mesh->addVertex(
        (Vertex){glm::normalize(glm::vec3(-a, a, -a)), glm::vec3(-a, a, a)});

    mesh->addTriangle(0, 1, 5);
    mesh->addTriangle(0, 5, 4);

    mesh->addTriangle(0, 2, 1);
    mesh->addTriangle(0, 3, 2);

    mesh->addTriangle(1, 2, 5);
    mesh->addTriangle(2, 6, 5);

    mesh->addTriangle(6, 2, 3);
    mesh->addTriangle(3, 7, 6);

    mesh->addTriangle(4, 5, 6);
    mesh->addTriangle(6, 7, 4);

    mesh->addTriangle(0, 4, 3);
    mesh->addTriangle(4, 7, 3);
  }

  ~SkyBox() { mesh.reset(); }

private:
  SkyBoxGeomType box_type;
  std::unique_ptr<Mesh> mesh;
};
} // namespace cruelEngine::cruelScene