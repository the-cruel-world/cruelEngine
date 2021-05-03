#include "application/application.hpp"
#include "window/window.hpp"

#include "render/render_header.h"
#include "render/vkcommon.h"
#include "scene/scene_header.h"
#include "ui/gui.hpp"

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

        {
            // Test RenderSessioin
            RenderSession session(context.get_instance(), context.get_device(), sessionProp);
            CRUEL_LOG("%s", "Create render Session\n");
        }

        RenderSession session(context.get_instance(), context.get_device(), sessionProp);

        {
            // Resource Cache
            ResourceCache render_resource_cache(context.get_device());
            std::size_t   hash_res = 0U;

            std::vector<LoadStoreInfo> load_store(
                10, {VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_STORE});

            hash_param(hash_res, load_store);

            CRUEL_LOG("Hash test %zu\n", hash_res);

            std::vector<VkAttachmentDescription> attachments{};
            std::vector<SubpassInfo> subPasses{};


            for (size_t i = 0; i < 10; i ++)
            {
                VkAttachmentDescription colorAttachment{};
                colorAttachment.format         = session.get_swapchain().get_surface_format().format;
                colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
                colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
                colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
                colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
                colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
                colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
                colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

                SubpassInfo              subpassInfo{};
                subpassInfo.input_attachments                = {};
                subpassInfo.output_attachments               = {0};
                subpassInfo.color_resolve_attachments        = {};
                subpassInfo.disable_depth_stencil_attachment = true;
                subpassInfo.depth_stencil_resolve_mode       = VK_RESOLVE_MODE_NONE;
                subpassInfo.depth_stencil_resolve_attachment = {};

                attachments.push_back(std::move(colorAttachment));
                subPasses.push_back(std::move(subpassInfo));

                std::size_t hash_val {0U};

                RenderPass &request = render_resource_cache.request_render_pass(attachments, subPasses);

                hash_param(hash_val, attachments, subPasses);

                printf("Got renderpass: %lu: at %p\n", hash_val, request.get_handle());
            }
        }

        {
            // Test RenderPass
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

        {
            // Test All
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
