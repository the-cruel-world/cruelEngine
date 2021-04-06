#pragma once
#include "../render_context.hpp"
#include "../render_session.hpp"
#include "../render_task.hpp"
#include "../vkcommon.h"
#include "../vulkan/buffer.hpp"
#include "../vulkan/buffer_pool.hpp"
#include "../vulkan/command_buffer.hpp"
#include "../vulkan/descriptor.hpp"
#include "../vulkan/pipeline.hpp"
#include "../vulkan/queue.hpp"
#include "../vulkan/render_pass.hpp"
#include "../vulkan/shader.hpp"
#include "../vulkan/swapchain.hpp"

namespace cruelEngine::cruelRender
{
// register new render tasks
class GeomTask : public RenderTask
{
public:
    GeomTask(RenderSession &session, std::shared_ptr<cruelScene::Object> object);

    void draw(cruelRender::CommandBuffer &commandBuffer, int index);

    void prepare();

    void prepare_assets();

    void prepare_pipeline(VkPipelineCache pipeline_cache, PipelineStatus &pipeline_state);

    void prepare_pipeline();

    void render();

    void update_render_pass(RenderPass &render_pass);

    //   void prepare_render_pass();

    struct UniformBufferObject
    {
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 proj;
    };

private:
    std::unique_ptr<DescriptorPool> descriptor_pool;

    std::vector<std::unique_ptr<cruelRender::DescriptorSet>> descriptor_set;

    std::unique_ptr<DescriptorSetLayout> descriptor_set_layout;

    std::unique_ptr<Buffer> vertex_buffer;

    std::unique_ptr<Buffer> index_buffer;

    std::unique_ptr<UniformBuffer> uniform_buffer;
};

class TriangleTest : public RenderTask
{
public:
    TriangleTest(RenderSession &session, std::shared_ptr<cruelScene::Object> object);

    void draw(cruelRender::CommandBuffer &commandBuffer, int index);

    void prepare();

    void prepare_pipeline(VkPipelineCache pipeline_cache, PipelineStatus &pipeline_state);

    void prepare_pipeline();

    void render();

    void update_render_pass(RenderPass &render_pass);

private:
};

} // namespace cruelEngine::cruelRender