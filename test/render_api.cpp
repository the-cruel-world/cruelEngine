#include "application/application.hpp"
#include "window/window.hpp"

#include "render/render_header.h"
#include "render/vkcommon.h"
#include "scene/scene_header.h"
#include "ui/gui.hpp"

using namespace std;
using namespace cruelEngine::cruelRender;

int main(int argc, char **argv)
{

    RenderProp context_prop =
        {
            .appInfo = {VK_STRUCTURE_TYPE_APPLICATION_INFO, "render_api", "Api test", USED_VULKAN_API_VERSION},
            .validation = true,
            .validationLayers = {"VK_LAYER_KHRONOS_validation"},
            .enabledInstanceExtensions = {},
            .enabledDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
            .features = {},
            .flags = {VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_TRANSFER_BIT | VK_QUEUE_COMPUTE_BIT},
            .overlay = false
        };
    RenderContext context (context_prop);
    cout << "Creating render Context." << endl;

    SessionProp sessionProp {
        .windowProp = {
            string(context_prop.appInfo.pApplicationName),
            1280,
            720,
            false
        },
        .render_features = RENDER_OPTION_DEFAULT_BIT,
        .frame_rate_limit = 100,
        .vsync = false
    };

    RenderSession session(context.get_instance(), context.get_device(), sessionProp);
    cout << "Creating render Session." << endl;

    return 0;
}