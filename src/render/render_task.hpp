#pragma once
#include "../scene/object.hpp"
#include "vkcommon.h"

namespace cruelEngine {

namespace cruelScene {
class Object;
}

namespace cruelRender {

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
  RenderTask(RenderSession &session, RenderPass &render_pass);

  virtual ~RenderTask();

  enum RenderTaskStatus {
    RENDER_TASK_UNINIRED = 0,
    RENDER_TASK_INIRED,
    RENDER_TASK_BUSY,
    RENDER_TASK_IDLE,
    RENDER_TASK_FINISHED,
    RENDER_TASK_DESTROY,
  };

  void prepare();

  void prepare_pipline_layout(const std::vector<ShaderModule> &shaders);

  void
  prepare_pipline_layout(const std::vector<ShaderModule> &shaders,
                         const VkPipelineLayoutCreateInfo &pipelineLayoutInfo);

  void prepare_pipeline(VkPipelineCache pipeline_cache,
                        PipelineStatus &pipeline_state);

  virtual void draw(cruelRender::CommandBuffer &commandBuffer, int index);

  virtual void render();

  RenderTaskStatus get_status() const { return status; }

  void set_status(RenderTaskStatus state) { status = state; }

protected:
  RenderSession &session;

  RenderPass &render_pass;

  std::unique_ptr<PipelineLayout> pipeline_layout{};

  std::unique_ptr<Pipeline> pipeline{};

  std::unique_ptr<RenderPass> renderpass{};

  std::shared_ptr<cruelScene::Object> object{};

  RenderTaskStatus status = RENDER_TASK_UNINIRED;
};
} // namespace cruelRender
} // namespace cruelEngine