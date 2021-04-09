#include "scene/mesh.hpp"
#include "scene/object.hpp"

namespace cruelEngine::cruelScene
{
class Camera;

enum class SkyBoxGeomType
{
    SKY_BOX_GEOM_CUBE = 0,
    SKY_BOX_GEOM_SPHERE,
};

class SkyBox : public Object
{
public:
    SkyBox(std::string name, Camera &camera) : Object(name, camera)
    {
        type     = ObjectType::SCENE_OBJ_TYPE_GEOM;
        box_type = SkyBoxGeomType::SKY_BOX_GEOM_SPHERE;

        mesh    = std::make_unique<Mesh>();
        float a = 0.5;

        mesh->addVertex((Vertex){glm::normalize(glm::vec3(a, a, a)), glm::vec3(a, a, -a)});
        mesh->addVertex((Vertex){glm::normalize(glm::vec3(a, -a, a)), glm::vec3(a, -a, -a)});
        mesh->addVertex((Vertex){glm::normalize(glm::vec3(-a, -a, a)), glm::vec3(-a, -a, -a)});
        mesh->addVertex((Vertex){glm::normalize(glm::vec3(-a, a, a)), glm::vec3(-a, a, -a)});

        mesh->addVertex((Vertex){glm::normalize(glm::vec3(a, a, -a)), glm::vec3(a, a, a)});
        mesh->addVertex((Vertex){glm::normalize(glm::vec3(a, -a, -a)), glm::vec3(a, -a, a)});
        mesh->addVertex((Vertex){glm::normalize(glm::vec3(-a, -a, -a)), glm::vec3(-a, -a, a)});
        mesh->addVertex((Vertex){glm::normalize(glm::vec3(-a, a, -a)), glm::vec3(-a, a, a)});

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

    ~SkyBox()
    {
        mesh.reset();
    }

    Mesh &get_mesh()
    {
        return *mesh;
    }

private:
    SkyBoxGeomType        box_type;
    std::unique_ptr<Mesh> mesh;
};
} // namespace cruelEngine::cruelScene