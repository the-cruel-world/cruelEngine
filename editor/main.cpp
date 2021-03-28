#include <algorithm>
#include <ctime>
#include <iostream>
#include <memory>
#include <vector>

#include "../src/application/application.hpp"
#include "../src/window/window.hpp"

#include "../src/render/render_header.h"
#include "../src/render/vkcommon.h"
#include "../src/scene/scene_header.h"

u32  cruelEngine::Window::count       = 0;
bool cruelEngine::Window::glfw_inited = false;

class Render : cruelEngine::Application
{
public:
    Render(size_t size)
    {
        std::cout << "frame time: " << frame_time << std::endl;

        render_context = std::make_unique<cruelEngine::cruelRender::RenderContext>(context_prop);
        std::cout << "Render context created." << std::endl;

        scene = std::make_shared<cruelEngine::cruelScene::Scene>();
        std::cout << "Scene created." << std::endl;

        scene->addObject(
            std::make_shared<cruelEngine::cruelScene::SkyBox>("Sky", scene->get_camera()));
        std::cout << "Scene object created." << std::endl;

        for (size_t i = 0; i < size; i++)
        {
            render_context->add_session(properties.window_prop.title, properties);
            render_context->get_session(i).load_scene(scene);
        }
        std::cout << "Scene object loaded." << std::endl;

        render_context->draw();
    }

    ~Render()
    {
        // scene.reset();
        render_context.reset();
    }

    void main_loop()
    {
        clock_t past_time   = clock();
        clock_t _frame_time = clock();
        while (render_context->is_context_alive())
        {
            glfwPollEvents();
            _frame_time = clock() - past_time;
            past_time   = clock();
            // std::cout << "frame time: " << _frame_time * 1e-3 << " ms\tusleep: " <<
            // (frame_time - _frame_time) * 1e-3 << " ms" << std::endl;
            if (_frame_time < frame_time)
                usleep(frame_time - _frame_time);
            render_context->render_frame();
        }
    }

private:
    cruelEngine::cruelRender::RenderProp context_prop = {
        .appInfo                   = {.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                    .pApplicationName = "Cruel Editor",
                    .pEngineName      = "Cruel Editor",
                    .apiVersion       = USED_VULKAN_API_VERSION},
        .validation                = true,
        .validationLayers          = {"VK_LAYER_KHRONOS_validation"},
        .enabledInstanceExtensions = {},
        .enabledDeviceExtensions   = {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
        .features                  = {.fillModeNonSolid = true, .wideLines = true},
        .flags                     = {},
        .vsync                     = false,
        .overlay                   = false,
    };
    cruelEngine::cruelRender::SessionProp properties = {{"Cruel Editor", 960, 640, false}};

    std::shared_ptr<cruelEngine::cruelScene::Scene> scene;

    std::unique_ptr<cruelEngine::cruelRender::RenderContext> render_context;

    u32 frame_time = 16e3;
};

int main(int argc, char const *argv[])
{
    Render(1).main_loop();

    return 0;
}