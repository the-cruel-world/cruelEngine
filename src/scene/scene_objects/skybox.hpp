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

    Mesh box;
    float a = 0.5;

    box.addVertex(
        (Vertex){glm::normalize(glm::vec3(a, a, a)), glm::vec3(a, a, -a)});
    box.addVertex(
        (Vertex){glm::normalize(glm::vec3(a, -a, a)), glm::vec3(a, -a, -a)});
    box.addVertex(
        (Vertex){glm::normalize(glm::vec3(-a, -a, a)), glm::vec3(-a, -a, -a)});
    box.addVertex(
        (Vertex){glm::normalize(glm::vec3(-a, a, a)), glm::vec3(-a, a, -a)});

    box.addVertex(
        (Vertex){glm::normalize(glm::vec3(a, a, -a)), glm::vec3(a, a, a)});
    box.addVertex(
        (Vertex){glm::normalize(glm::vec3(a, -a, -a)), glm::vec3(a, -a, a)});
    box.addVertex(
        (Vertex){glm::normalize(glm::vec3(-a, -a, -a)), glm::vec3(-a, -a, a)});
    box.addVertex(
        (Vertex){glm::normalize(glm::vec3(-a, a, -a)), glm::vec3(-a, a, a)});

    box.addTriangle(0, 1, 5);
    box.addTriangle(0, 5, 4);

    box.addTriangle(0, 2, 1);
    box.addTriangle(0, 3, 2);

    box.addTriangle(1, 2, 5);
    box.addTriangle(2, 6, 5);

    box.addTriangle(6, 2, 3);
    box.addTriangle(3, 7, 6);

    box.addTriangle(4, 5, 6);
    box.addTriangle(6, 7, 4);

    box.addTriangle(0, 4, 3);
    box.addTriangle(4, 7, 3);

    mesh.push_back(std::move(box));
  }

  ~SkyBox() = default;

private:
  SkyBoxGeomType box_type;
};
} // namespace cruelEngine::cruelScene