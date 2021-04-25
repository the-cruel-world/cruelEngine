//
// Created by yiwen on 4/24/21.
//

#include "render/subpasses/geompass.hpp"
#include "render/render_session.hpp"
#include "render/vulkan/command_buffer.hpp"

#include "scene/component.hpp"
#include "scene/components/camera.hpp"
#include "scene/components/light.hpp"
#include "scene/components/mesh.hpp"
#include "scene/components/primitive.hpp"
#include "scene/node.hpp"
#include "scene/scene.hpp"

namespace cruelEngine::cruelRender
{
GeomPass::GeomPass(RenderSession &session, cruelScene::Scene &scene) :
    SubPass{session}, scene{scene}
{}

void GeomPass::prepare()
{}

void GeomPass::draw(CommandBuffer &commandBuffer)
{
    auto &tasks = session.GetTasks();
    for (auto &task : session.GetTasks())
    {
    }
}

void GeomPass::DrawPrimitives(CommandBuffer &commandBuffer)
{}

} // namespace cruelEngine::cruelRender