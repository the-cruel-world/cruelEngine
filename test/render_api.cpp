#include "application/application.hpp"
#include "window/window.hpp"

#include "render/render_header.h"
#include "render/vkcommon.h"
#include "scene/scene_header.h"
#include "ui/gui.hpp"

#ifndef RESOURCE_BASE_DIR
#define RESOURCE_BASE_DIR "Hello"
#endif

using namespace std;
using namespace cruelEngine::cruelRender;

u32  cruelEngine::Window::count       = 0;
bool cruelEngine::Window::glfw_inited = false;

int main(int argc, char **argv)
{
    {
        CRUEL_LOG("%s\n", "Launching");
        RenderProp context_prop = {.appInfo          = {.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                                               .pApplicationName = "API test",
                                               .pEngineName      = "cruelEngine",
                                               .apiVersion       = USED_VULKAN_API_VERSION},
                                   .validation       = true,
                                   .validationLayers = {"VK_LAYER_KHRONOS_validation"},
                                   .enabledInstanceExtensions = {},
                                   .enabledDeviceExtensions   = {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
                                   .features                  = {},
                                   .flags = VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT |
                                            VK_QUEUE_COMPUTE_BIT,
                                   .overlay = false};
        // RenderContext test
        {
            RenderContext context(context_prop);
            CRUEL_LOG("%s", "Create render Context\n");
        }

        RenderContext context(context_prop);
        CRUEL_LOG("%s", "Create render Context\n");

        SessionProp sessionProp{
            .windowProp       = {string(context_prop.appInfo.pApplicationName), 1280, 720, false},
            .render_features  = RENDER_OPTION_DEFAULT_BIT,
            .frame_rate_limit = 100,
            .vsync            = false};

        // Test RenderSessioin
        {
            RenderSession session(context.get_instance(), context.get_device(), sessionProp);
            CRUEL_LOG("%s", "Create render Session\n");
        }

        RenderSession session(context.get_instance(), context.get_device(), sessionProp);

        // Resource Cache test
        {
            ResourceCache render_resource_cache(context.get_device());

            // RenderPass from cache test
            {
                std::vector<VkAttachmentDescription> attachments{};
                std::vector<SubpassInfo>             subPasses{};
                for (size_t i = 0; i < 10; i++)
                {
                    VkAttachmentDescription colorAttachment{};
                    colorAttachment.format  = session.get_swapchain().get_surface_format().format;
                    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
                    colorAttachment.loadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR;
                    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
                    colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                    colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
                    colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

                    SubpassInfo subpassInfo{};
                    subpassInfo.input_attachments                = {};
                    subpassInfo.output_attachments               = {0};
                    subpassInfo.color_resolve_attachments        = {};
                    subpassInfo.disable_depth_stencil_attachment = true;
                    subpassInfo.depth_stencil_resolve_mode       = VK_RESOLVE_MODE_NONE;
                    subpassInfo.depth_stencil_resolve_attachment = {};

                    attachments.push_back(std::move(colorAttachment));
                    subPasses.push_back(std::move(subpassInfo));

                    std::size_t hash_val{0U};

                    RenderPass &request =
                        render_resource_cache.request_render_pass(attachments, subPasses);

                    hash_param(hash_val, attachments, subPasses);

                    printf("  Got renderpass: %lu: at %p\n", hash_val, request.get_handle());
                }
            }

            // Descriptorpool test
            {
                std::vector<VkDescriptorSetLayoutBinding> bindings = {};
                for (size_t i = 1; i < 10; i++)
                {
                    CRUEL_LOG("Descriptor from cache test round %zu\n", i);
                    std::size_t hash_val{0U};

                    VkDescriptorSetLayoutBinding uboLayoutBinding{};
                    uboLayoutBinding.binding                           = i;
                    uboLayoutBinding.descriptorType                    = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    uboLayoutBinding.descriptorCount                   = 1;
                    uboLayoutBinding.stageFlags                        = VK_SHADER_STAGE_VERTEX_BIT;
                    uboLayoutBinding.pImmutableSamplers                = nullptr; // Optional

                    bindings.push_back(uboLayoutBinding);
                    DescriptorSetLayout &layout = render_resource_cache.request_descriptor_set_layout(bindings);
                    hash_val = 0U;
                    hash_param(hash_val, bindings);
                    printf("  Got descriptorSetlayout: %lu: at %p\n", hash_val, layout.get_handle());

                    int descriptor_count = bindings.size();
                    VkDescriptorPoolSize poolSize{};
                    poolSize.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    poolSize.descriptorCount = descriptor_count;

                    DescriptorPool &pool = render_resource_cache.request_descriptor_pool(poolSize);
                    hash_val = 0U;
                    hash_param(hash_val, poolSize, DescriptorPool::MAX_SETS_PER_POOL);
                    printf("  Got descriptorPool: %lu: at %p\n", hash_val, pool.get_handle());

                    DescriptorSet &set = render_resource_cache.request_descriptor_set(layout, pool);
                    hash_val = 0U;
                    hash_param(hash_val, layout, pool);
                    printf("  Got descriptorSet: %lu: at %p\n", hash_val, set.get_handle());
                }
            }

            // Pipeline test
            {
                for (size_t i = 1; i < 10; i++)
                {
                    CRUEL_LOG("Pipeline from cache test round %zu\n", i);
                    std::size_t hash_val{0U};

                    std::vector<ShaderModule> shaders{};
                    shaders.emplace_back(session.get_device(),
                                         RESOURCE_BASE_DIR"/shaders/"
                                         "wireframe/frag.spv",
                                         "main", VK_SHADER_STAGE_FRAGMENT_BIT);

                    shaders.emplace_back(session.get_device(),
                                         RESOURCE_BASE_DIR"/shaders/"
                                         "wireframe/vert.spv",
                                         "main", VK_SHADER_STAGE_VERTEX_BIT);

                    VkDescriptorSetLayoutBinding uboLayoutBinding{};
                    uboLayoutBinding.binding                           = 0;
                    uboLayoutBinding.descriptorType                    = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
                    uboLayoutBinding.descriptorCount                   = 1;
                    uboLayoutBinding.stageFlags                        = VK_SHADER_STAGE_VERTEX_BIT;
                    uboLayoutBinding.pImmutableSamplers                = nullptr; // Optional
                    DescriptorSetLayout &descriptor_set_layout = render_resource_cache.request_descriptor_set_layout({uboLayoutBinding});

                    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
                    pipelineLayoutInfo.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
                    pipelineLayoutInfo.setLayoutCount         = 1;
                    pipelineLayoutInfo.pSetLayouts            = &descriptor_set_layout.get_handle(); // Optional
                    pipelineLayoutInfo.pushConstantRangeCount = 0;                                    // Optional
                    pipelineLayoutInfo.pPushConstantRanges    = nullptr;                              // Optional

                    PipelineLayout &pipelineLayout = render_resource_cache.request_pipeline_layout(shaders, pipelineLayoutInfo);
                    hash_val = 0U;
                    hash_param(hash_val, shaders, pipelineLayoutInfo);
                    printf("  Got PipelineLayout: %lu: at %p\n", hash_val, pipelineLayout.get_handle());

                    shaders.clear();
                    shaders.emplace_back(session.get_device(),
                                         RESOURCE_BASE_DIR"/shaders/"
                                         "triangle/hello_triangle/frag.spv",
                                         "main", VK_SHADER_STAGE_FRAGMENT_BIT);

                    shaders.emplace_back(session.get_device(),
                                         RESOURCE_BASE_DIR"/shaders/"
                                         "triangle/hello_triangle/vert.spv",
                                         "main", VK_SHADER_STAGE_VERTEX_BIT);

                    pipelineLayoutInfo.setLayoutCount         = 0;
                    pipelineLayoutInfo.pSetLayouts            = nullptr; // Optional
                    PipelineLayout &pipelineLayout2 = render_resource_cache.request_pipeline_layout(shaders, pipelineLayoutInfo);


                    VkAttachmentDescription colorAttachment{};
                    colorAttachment.format  = session.get_swapchain().get_surface_format().format;
                    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
                    colorAttachment.loadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR;
                    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
                    colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                    colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
                    colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

                    SubpassInfo subpassInfo{};
                    subpassInfo.input_attachments                = {};
                    subpassInfo.output_attachments               = {0};
                    subpassInfo.color_resolve_attachments        = {};
                    subpassInfo.disable_depth_stencil_attachment = true;
                    subpassInfo.depth_stencil_resolve_mode       = VK_RESOLVE_MODE_NONE;
                    subpassInfo.depth_stencil_resolve_attachment = {};

                    RenderPass &renderPass =
                        render_resource_cache.request_render_pass({colorAttachment}, {subpassInfo});

                    PipelineStatus                   pipeline_state{};
                    PipelineStatus::VertexInputState vertex_input_state;

                    PipelineStatus::InputAssemblyState input_assembly_state;
                    PipelineStatus::RasterizationState rasterization_state = {
                        VK_FALSE, VK_FALSE, VK_POLYGON_MODE_FILL, VK_CULL_MODE_BACK_BIT, VK_FRONT_FACE_CLOCKWISE,
                        VK_FALSE};
                    PipelineStatus::ViewportState    viewport_state;
                    PipelineStatus::MultisampleState multisample_state = {
                        VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.0f, 0, VK_FALSE, VK_FALSE};

                    PipelineStatus::DepthStencilState         depth_stencil_state;
                    PipelineStatus::ColorBlendState           color_blend_state;
                    PipelineStatus::ColorBlendAttachmentState attach = {
                        VK_FALSE,
                        VK_BLEND_FACTOR_ONE,
                        VK_BLEND_FACTOR_ZERO,
                        VK_BLEND_OP_ADD,
                        VK_BLEND_FACTOR_ONE,
                        VK_BLEND_FACTOR_ZERO,
                        VK_BLEND_OP_ADD,
                        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                        VK_COLOR_COMPONENT_A_BIT};
                    color_blend_state.attachments.push_back(attach);

                    PipelineStatus::DynamicState dynamic_state;
                    pipeline_state.set_vertex_input_state(vertex_input_state);
                    pipeline_state.set_input_assembly_state(input_assembly_state);
                    pipeline_state.set_rasterization_state(rasterization_state);
                    pipeline_state.set_viewport_state(viewport_state);
                    pipeline_state.set_multisample_state(multisample_state);
                    pipeline_state.set_depth_stencil_state(depth_stencil_state);
                    pipeline_state.set_color_blend_state(color_blend_state);
                    pipeline_state.set_dynamic_state(dynamic_state);
                    pipeline_state.set_pipeline_layout(pipelineLayout2);
                    pipeline_state.set_render_pass(renderPass);
                    pipeline_state.set_subpass_index(0);

                    GraphicsPipeline &graphicsPipeline = render_resource_cache.request_graphics_pipeline(pipeline_state);
                    hash_val = 0U;
                    hash_param(hash_val, pipeline_state);
                    printf("  Got GraphicsPipeline: %lu: at %p\n", hash_val, graphicsPipeline.get_handle());
                }
            }
            render_resource_cache.summary();
        }

        // RenderPass test
        {
            std::vector<VkAttachmentDescription> attachments{};

            VkAttachmentDescription colorAttachment{};
            colorAttachment.format         = session.get_swapchain().get_surface_format().format;
            colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
            colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
            colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
            colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
            colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
            colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
            colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

            std::vector<SubpassInfo> subPasses{};
            SubpassInfo              subpassInfo{};
            subpassInfo.input_attachments                = {};
            subpassInfo.output_attachments               = {0};
            subpassInfo.color_resolve_attachments        = {};
            subpassInfo.disable_depth_stencil_attachment = true;
            subpassInfo.depth_stencil_resolve_mode       = VK_RESOLVE_MODE_NONE;
            subpassInfo.depth_stencil_resolve_attachment = {};

            attachments.push_back(std::move(colorAttachment));
            subPasses.push_back(std::move(subpassInfo));

            RenderPass renderPass(context.get_device(), attachments, subPasses);
        }

        // Test All
        {
            RenderPipeline pipeline(context.get_device());
            CRUEL_LOG("%s", "Create render pipeline\n");

            auto lightPass = std::make_unique<LightPass>(session);
            lightPass->set_output_attachments({1});

            auto shadowPass = std::make_unique<LightPass>(session);
            lightPass->set_input_attachments({1});
            lightPass->set_output_attachments({0});

            pipeline.AddSubpass(std::move(lightPass));
            pipeline.AddSubpass(std::move(shadowPass));

            auto swapchain_images = session.get_swapchain().get_images();
            std::vector<std::unique_ptr<RenderTarget>> renderTargets;

            for (std::size_t i = 0; i < swapchain_images.size(); i++)
            {
                std::vector<Image> images;
                VkExtent3D         extent{session.get_swapchain().get_extent().width,
                                  session.get_swapchain().get_extent().height, 1};

                Image swapchain_image(context.get_device(), swapchain_images[i], extent,
                                      session.get_swapchain().get_surface_format().format,
                                      session.get_swapchain().get_properties().image_usage);

                Image normal_image{session.get_device(),
                                   extent,
                                   VK_FORMAT_R32G32B32A32_SFLOAT,
                                   VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
                                   VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                   VK_SAMPLE_COUNT_1_BIT,
                                   1,
                                   1,
                                   VK_IMAGE_TILING_OPTIMAL};

                // attachment 0
                images.push_back(std::move(swapchain_image));
                // attachment 1
                images.push_back(std::move(normal_image));

                auto renderTarget =
                    std::make_unique<RenderTarget>(context.get_device(), std::move(images));

                renderTargets.push_back(std::move(renderTarget));
            }

            RenderTarget &renderTarget = *renderTargets.at(0);

            RenderFrame frame;
            frame.AddTargets(std::move(renderTargets));

            renderTarget.set_load_store_op(
                {{VK_ATTACHMENT_LOAD_OP_LOAD, VK_ATTACHMENT_STORE_OP_STORE},
                 {VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE}});
            renderTarget.set_stencil_load_store_op(
                {{VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE},
                 {VK_ATTACHMENT_LOAD_OP_DONT_CARE, VK_ATTACHMENT_STORE_OP_DONT_CARE}});

            pipeline.Draw(
                session.getCommandPool().RequestCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY),
                renderTarget);
            sleep(1);
        }
    }
    CRUEL_LOG("%s\n", "Terminating");
    return 0;
}
