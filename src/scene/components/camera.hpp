#pragma once
#include "../component.hpp"

namespace cruelEngine::cruelScene
{
class Camera : public Component
{
public:
    Camera(const std::string &name);

    virtual ~Camera();

    virtual glm::mat4 get_projection()
    {
        return glm::mat4(1.0f);
    }

    glm::mat4 get_view()
    {
        return glm::mat4(1.0f);
    }

    float get_aspect_ratio() const
    {
        return aspect_ratio;
    }
    void set_aspect_ratio(float ratio);

    float get_fov() const
    {
        return fov;
    }
    void set_fov(float fov){};

    float get_far_plane() const
    {
        return far_plane;
    }
    void set_far_plane(float zfar){};

    float get_near_plane() const
    {
        return near_plane;
    }
    void set_near_plane(float znear){};

private:
    float aspect_ratio{1.0f};

    float fov{glm::radians(60.0f)};

    float far_plane{100.0};

    float near_plane{0.1f};
};
} // namespace cruelEngine::cruelScene