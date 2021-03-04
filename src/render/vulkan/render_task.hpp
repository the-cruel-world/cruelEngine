#pragma once
#include "vkcommon.h"

namespace cruelEngine {
namespace VulkanContext {

    class Pipeline;
    class PipelineStatus;
    class PipelineLayout;
    class GraphicsPipeline;
    class RenderPass;
    class RenderSession;
    class CommandBuffer;
    class Queue;
    class ShaderModule;

    class RenderTask {
    public:
        RenderTask (RenderSession &session, RenderPass &render_pass);
        
        ~RenderTask ();

        enum RenderTaskStatus{
            RENDER_TASK_UNINIRED = 0,
            RENDER_TASK_INIRED,
            RENDER_TASK_BUSY,
            RENDER_TASK_IDLE,
            RENDER_TASK_FINISHED,
            RENDER_TASK_DESTROY,
        };

        void                prepare();

        void                prepare_pipline_layout(const std::vector<ShaderModule> &shaders);

        void                prepare_pipeline(VkPipelineCache pipeline_cache, PipelineStatus  &pipeline_state);

        void                draw(CommandBuffer &commandBuffer);

        RenderTaskStatus    get_status() const {return status;}

        void                set_status(RenderTaskStatus state) {status = state;}

    private:
        RenderSession   &session;

        RenderPass      &render_pass;

        std::unique_ptr<PipelineLayout> pipeline_layout;

        std::unique_ptr<Pipeline>       pipeline;

        RenderTaskStatus                status = RENDER_TASK_UNINIRED;
    };
}
}