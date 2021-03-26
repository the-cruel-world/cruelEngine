#pragma once
#include "../scene/object.hpp"
#include "vkcommon.h"

namespace cruelEngine
{
namespace cruelScene
{
class Object;
}

namespace cruelRender
{
class Pipeline;
class PipelineStatus;
class PipelineLayout;
class GraphicsPipeline;
class RenderPass;
class RenderSession;
class CommandBuffer;
class Queue;
class ShaderModule;

class RenderTask
{
public:
    RenderTask(RenderSession &session, std::shared_ptr<cruelScene::Object> object);

    RenderTask(const RenderSession &) = delete;

    RenderTask(RenderTask &&) = delete;

    RenderTask &operator=(const RenderTask &) = delete;

    RenderTask &operator=(RenderTask &&) = delete;

    virtual ~RenderTask();

    enum class RenderTaskStatus
    {
        RENDER_TASK_UNINIRED = 0,
        RENDER_TASK_INIRED,
        RENDER_TASK_BUSY,
        RENDER_TASK_IDLE,
        RENDER_TASK_FINISHED,
        RENDER_TASK_DESTROY,
    };

    virtual void prepare() = 0;

    void prepare_pipeline_layout(const std::vector<ShaderModule> &shaders);

    void prepare_pipeline_layout(const std::vector<ShaderModule> & shaders,
                                 const VkPipelineLayoutCreateInfo &pipelineLayoutInfo);

    virtual void prepare_pipeline() = 0;

    virtual void prepare_pipeline(VkPipelineCache pipeline_cache, PipelineStatus &pipeline_state);

    virtual void draw(cruelRender::CommandBuffer &commandBuffer, int index);

    virtual void update_render_pass(RenderPass &render_pass) = 0;

    virtual void render();

    RenderTaskStatus get_status() const
    {
        return status;
    }

    void set_status(RenderTaskStatus state)
    {
        status = state;
    }

protected:
    RenderSession &session;

    // RenderPass &render_pass;

    std::unique_ptr<PipelineLayout> pipeline_layout{};

    std::unique_ptr<Pipeline> pipeline{};

    // std::unique_ptr<RenderPass> render_pass{};

    std::shared_ptr<cruelScene::Object> object{};

    RenderTaskStatus status = RenderTaskStatus::RENDER_TASK_UNINIRED;
};
} // namespace cruelRender
} // namespace cruelEngine