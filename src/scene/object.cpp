#include "object.hpp"
#include "mesh.hpp"
#include "../render/vulkan/render_task.hpp"
#include "../render/vulkan/render.hpp"
#include "../render/vulkan/render_session.hpp"
#include "../render/vulkan/shader.hpp"
#include "../render/vulkan/logical_device.hpp"
#include "../render/vulkan/pipeline.hpp"
#include "../render/vulkan/swapchain.hpp"


namespace cruelEngine {
namespace cruelScene {
    Object::Object(VulkanContext::VulkanContext &render_context, std::string name) :
        render_context {render_context}, name {name}
    {
        show_name();
        task = render_context.get_session().request_new_task();

        shaders.emplace_back(
            render_context.get_device(), 
            "/home/yiwen/program/cruelworld/engine/test/frag.spv", 
            "main", 
            VK_SHADER_STAGE_FRAGMENT_BIT);

        shaders.emplace_back(
            render_context.get_device(), 
            "/home/yiwen/program/cruelworld/engine/test/vert.spv", 
            "main", 
            VK_SHADER_STAGE_VERTEX_BIT);

        std::cout << "[" << name << "] Shader Created." << std::endl;

        task->prepare_pipline_layout(shaders);
        std::cout << "[" << name << "] PipelineLayout Created." << std::endl;

        // task->prepare_render_pass();
        // std::cout << "[" << name << "] Render Pass Created." << std::endl;

        VulkanContext::PipelineStatus pipeline_state {};
        VulkanContext::PipelineStatus::VertexInputState     vertex_input_state;
        VulkanContext::PipelineStatus::InputAssemblyState   input_assembly_state;
        VulkanContext::PipelineStatus::RasterizationState   rasterization_state
        = {
            VK_FALSE, 
            VK_FALSE, 
            VK_POLYGON_MODE_FILL, 
            VK_CULL_MODE_BACK_BIT, 
            VK_FRONT_FACE_CLOCKWISE, 
            VK_FALSE
        };
        VulkanContext::PipelineStatus::ViewportState        viewport_state;
        // VkViewport viewport = 
        // {
        //     .x = 0.0f,
        //     .y = 0.0f,
        //     .height = (float) render_context.get_session().get_swapchain().get_extent().height,
        //     .width = (float) render_context.get_session().get_swapchain().get_extent().height,
        //     .minDepth = 0.0f,
        //     .maxDepth = 1.0f
        // };
        // VkRect2D scissor = 
        // {
        //     .offset = {0, 0},
        //     .extent = render_context.get_session().get_swapchain().get_extent()
        // };
        // viewport_state.viewport.push_back(viewport);
        // viewport_state.scissor.push_back(scissor);

        VulkanContext::PipelineStatus::MultisampleState     multisample_state 
        = {
            VK_SAMPLE_COUNT_1_BIT,
            VK_FALSE,
            1.0f,
            VK_NULL_HANDLE,
            VK_FALSE,
            VK_FALSE
        };
        VulkanContext::PipelineStatus::DepthStencilState    depth_stencil_state;
        VulkanContext::PipelineStatus::ColorBlendState      color_blend_state;
        VulkanContext::PipelineStatus::ColorBlendAttachmentState attach
         = {VK_FALSE, 
         VK_BLEND_FACTOR_ONE, 
         VK_BLEND_FACTOR_ZERO, 
         VK_BLEND_OP_ADD, 
         VK_BLEND_FACTOR_ONE, 
         VK_BLEND_FACTOR_ZERO, 
         VK_BLEND_OP_ADD, 
         VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT};
        color_blend_state.attachments.push_back(attach);

        pipeline_state.set_vertex_input_state(vertex_input_state);
        pipeline_state.set_input_assembly_state(input_assembly_state);
        pipeline_state.set_rasterization_state(rasterization_state);
        pipeline_state.set_viewport_state(viewport_state);
        pipeline_state.set_multisample_state(multisample_state);
        pipeline_state.set_depth_stencil_state(depth_stencil_state);
        pipeline_state.set_color_blend_state(color_blend_state);
        pipeline_state.set_subpass_index(0);

        task->prepare_pipeline(VK_NULL_HANDLE, pipeline_state);
        std::cout << "[" << name << "] Pipeline Created." << std::endl;
    }

    Object::~Object()
    {

    }

    void Object::prepare ()
    {

    }

    void Object::drawCommand ()
    {
        
    }

}
}