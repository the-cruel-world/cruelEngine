#include "object.hpp"

#include "../render/render_context.hpp"
#include "../render/render_session.hpp"
#include "../render/render_task.hpp"
#include "../render/vulkan/logical_device.hpp"
#include "../render/vulkan/pipeline.hpp"
#include "../render/vulkan/shader.hpp"
#include "../render/vulkan/swapchain.hpp"
#include "mesh.hpp"

namespace cruelEngine {
namespace cruelScene {
Object::Object(cruelRender::RenderContext &render_context, u32 session_idx,
               std::string name)
    : name{name}, session_idx{session_idx}, render_context{render_context} {
  show_name();
}

Object::~Object() {}

void Object::prepare() {}

void Object::drawCommand() {}
} // namespace cruelScene
} // namespace cruelEngine