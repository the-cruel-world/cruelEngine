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
    CRUEL_LOG("%s\n", "Launching");
    RenderProp    context_prop = {.appInfo          = {.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
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
    RenderContext context(context_prop);
    CRUEL_LOG("%s", "Create render Context\n");

    SessionProp sessionProp{
        .windowProp       = {string(context_prop.appInfo.pApplicationName), 1280, 720, false},
        .render_features  = RENDER_OPTION_DEFAULT_BIT,
        .frame_rate_limit = 100,
        .vsync            = false};

    RenderSession session(context.get_instance(), context.get_device(), sessionProp);
    CRUEL_LOG("%s", "Create render Session\n");

    RenderPipeline pipeline(context.get_device());
    CRUEL_LOG("%s", "Create render pipeline\n");

    auto lightPass = std::make_unique<LightPass>(session);

    lightPass->set_input_attachments({0});

    pipeline.AddSubpass(std::move(lightPass));

    std::vector<Image> images;
    VkExtent3D         extent{session.get_swapchain().get_extent().width,
                      session.get_swapchain().get_extent().height, 1};
    Image swapchain_image(context.get_device(), session.get_swapchain().get_images()[0], extent,
                          session.get_swapchain().get_surface_format().format,
                          session.get_swapchain().get_properties().image_usage);

    // attachment 0
    images.push_back(std::move(swapchain_image));
    RenderTarget renderTarget(context.get_device(), std::move(images));

    pipeline.Draw(session.getCommandPool().RequestCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY),
                  renderTarget);

    sleep(1);

    CRUEL_LOG("%s\n", "Terminating");
    return 0;
}
