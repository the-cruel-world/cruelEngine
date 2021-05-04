//
// Created by yiwen on 4/24/21.
//

#include "render/subpasses/geompass.hpp"
#include "render/render_context.hpp"
#include "render/render_session.hpp"
#include "render/resource_cache.hpp"
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
GeomPass::GeomPass(RenderContext &context, cruelScene::Scene &scene, cruelScene::Camera &camera) :
    SubPass{context}, scene{scene}
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
    PipelineStatus::RasterizationState rasterizationState{};
    rasterizationState.front_face = frontFace;
    commandBuffer.set_rasterization_state(rasterizationState);

    std::vector<ShaderModule> a;
    PipelineLayout &pipelineLayout = context.GetResourceCache().request_pipeline_layout({});
    commandBuffer.set_pipeline_layout(pipelineLayout);
    // invoke render pipeline for primitives

    // \todo needs redesign
    PipelineStatus::VertexInputState vertex_input{};
    for (auto &a : primitive.GetVertexAttributes())
    {
        VkVertexInputAttributeDescription attribute{};
        VkVertexInputBindingDescription   binding{};

        binding.binding = 0;
        binding.stride  = a.stride;

        attribute.binding  = 0;
        attribute.offset   = a.offset;
        attribute.format   = VK_FORMAT_R32G32B32_SFLOAT;
        attribute.location = 0;

        vertex_input.bindings.push_back(binding);
        vertex_input.attributes.push_back(attribute);
    }

    // draw primitives.
}

} // namespace cruelEngine::cruelRender