#include "camera.hpp"

namespace cruelEngine::cruelScene
{
Camera::Camera(const std::string &name) : Component{name}
{}

Camera::~Camera()
{}
std::type_index Camera::GetType()
{
    return typeid(Camera);
}

} // namespace cruelEngine::cruelScene