//
// Created by yiwen on 4/24/21.
//

#ifndef CRUELENGINE_GEOMPASS_HPP
#define CRUELENGINE_GEOMPASS_HPP

#pragma once
#include "render/subpass.hpp"

namespace cruelEngine::cruelScene
{
class Scene;
class Camera;
class Primitive;
class Mesh;
class Scene;

} // namespace cruelEngine::cruelScene

namespace cruelEngine::cruelRender
{
class GeomPass : public SubPass
{
public:
    GeomPass(RenderSession &session, cruelScene::Scene &scene, cruelScene::Camera &camera);

    virtual void prepare() override;

    virtual void prepare_pipeline();

    virtual void draw(CommandBuffer &commandBuffer) override;

protected:
    void UpdateUniform();

    void DrawPrimitives(CommandBuffer &commandBuffer, cruelScene::Primitive &primitive,
                        VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE);

    cruelScene::Scene &scene;

private:
};
} // namespace cruelEngine::cruelRender
#endif // CRUELENGINE_GEOMPASS_HPP
