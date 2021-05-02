//
// Created by yiwen on 4/24/21.
//

#include "render/subpasses/geompass.hpp"
#include "render/render_session.hpp"
#include "render/vulkan/command_buffer.hpp"
#include "render/vulkan/pipeline.hpp"

#include "scene/component.hpp"
#include "scene/components/camera.hpp"
#include "scene/components/light.hpp"
#include "scene/components/mesh.hpp"
#include "scene/components/primitive.hpp"
#include "scene/node.hpp"
#include "scene/scene.hpp"

namespace cruelEngine::cruelRender
{
GeomPass::GeomPass(RenderSession &session, cruelScene::Scene &scene, cruelScene::Camera &camera) :
    SubPass{session}, scene{scene}
{}

void GeomPass::prepare()
{}

void GeomPass::prepare_pipeline()
{}
void GeomPass::UpdateUniform()
{}
void GeomPass::draw(CommandBuffer &commandBuffer)
{
    auto all_primitive = scene.GetComponents<cruelScene::Primitive>();

    for (auto &primitive : all_primitive)
    {
        UpdateUniform();

        DrawPrimitives(commandBuffer, *primitive, VK_FRONT_FACE_CLOCKWISE);
    }
}

void GeomPass::DrawPrimitives(CommandBuffer &commandBuffer, cruelScene::Primitive &primitive,
                              VkFrontFace frontFace)
{
    // prepare pipelineStatus
    pipelineStatus;

    // invoke render pipeline for primitives

    // draw primitives.
}

} // namespace cruelEngine::cruelRender