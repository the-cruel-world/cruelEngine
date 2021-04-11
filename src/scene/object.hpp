#pragma once

#include "sgcommon.h"

namespace cruelEngine::cruelScene
{
struct Texture;
struct Vertex;
class Camera;
class Mesh;

enum class ObjectType
{
    SCENE_OBJ_TYPE_GEOM = 0,
    SCENE_OBJ_TYPE_CUBE,
    SCENE_OBJ_TYPE_SPHERE,
};

class Object
{
public:
    Object(std::string name, Camera &camera);

    virtual ~Object();

    // virtual void prepare() = 0;

    // virtual void loadAsset(){};

    // virtual void unloadAsset(){};

    const std::string &get_name() const
    {
        return name;
    }

    const ObjectType get_type() const
    {
        return type;
    }

    virtual Mesh &get_mesh() = 0;

    virtual Camera &get_camera()
    {
        return camera;
    }

    // bool is_alive() const;

    // void show_name() const { std::cout << "Object : " << name << std::endl; }

protected:
    std::string name;

    ObjectType type;

    Camera &camera;

    // std::vector<Mesh1> mesh;
    // std::vector<Texture> textures;
    // std::vector<Vertex> vertices;
};
} // namespace cruelEngine::cruelScene