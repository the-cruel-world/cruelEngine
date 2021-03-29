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
#include "../src/ui/gui.hpp"

u32  cruelEngine::Window::count       = 0;
bool cruelEngine::Window::glfw_inited = false;

using namespace cruelEngine;

void updateOverlay(void *gui);

class Render : cruelEngine::Application
{
public:
    Render(size_t size)
    {
#ifdef DEBUG
        std::cout << "frame time: " << frame_time << std::endl;
#endif

        render_context = std::make_unique<cruelEngine::cruelRender::RenderContext>(context_prop);
#ifdef DEBUG
        std::cout << "Render context created." << std::endl;
#endif

        properties.window_prop.width  = 640;
        properties.window_prop.height = 480;
        properties.window_prop.title  = "cruelEngine::Triangle::new Window";

        scene = std::make_shared<cruelEngine::cruelScene::Scene>();
#ifdef DEBUG
        std::cout << "Scene created." << std::endl;
#endif
        scene->addObject(
            std::make_shared<cruelEngine::cruelScene::SkyBox>("Sky", scene->get_camera()));
#ifdef DEBUG
        std::cout << "Scene object created." << std::endl;
#endif

        for (size_t i = 0; i < size; i++)
        {
            render_context->add_session(properties.window_prop.title, properties);
            render_context->get_session(i).load_scene(scene);
            render_context->get_session(i).setGuiOverlay(
                std::make_shared<cruelGui::Gui>(render_context->get_session(i)));
            render_context->get_session(i).setGuiOverlayUpdateCb(updateOverlay);
        }
#ifdef DEBUG
        std::cout << "Scene object loaded." << std::endl;
#endif
        render_context->draw();
    }

    ~Render()
    {
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
            std::cout << "frame time: " << _frame_time * 1e-3
                      << " ms\tusleep: " << (frame_time - _frame_time) * 1e-3 << " ms" << std::endl;
            if (_frame_time < frame_time)
                usleep(frame_time - _frame_time);
            render_context->render_frame();
        }
    }

private:
    cruelEngine::cruelRender::RenderProp context_prop = {
        .appInfo                   = {.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                    .pApplicationName = "Cruel Engine",
                    .pEngineName      = "Cruel Engine",
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
    cruelEngine::cruelRender::SessionProp properties = {
        {"cruelEngine::Triangle::main Window", 720, 640, false}};

    std::shared_ptr<cruelEngine::cruelScene::Scene> scene;

    std::unique_ptr<cruelEngine::cruelRender::RenderContext> render_context;

    u32 frame_time = 16e3;
};

void updateOverlay(void *gui)
{
    auto UIOverlay = (cruelGui::Gui *) gui;
    if (UIOverlay == nullptr)
        std::cerr << "[Render::main_loop::updateOverlay] updating guiOverlay" << std::endl;

    ImGuiIO &io    = ImGui::GetIO();
    io.DisplaySize = ImVec2(UIOverlay->getSession().get_swapchain().get_properties().extent.width,
                            UIOverlay->getSession().get_swapchain().get_properties().extent.height);

    ImGui::NewFrame();

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 2);
    ImGui::Begin("magicDick", nullptr, ImGuiWindowFlags_NoMove);
    ImGui::PushItemWidth(110.0f * UIOverlay->getScale());
    UIOverlay->header("Hey There");

    UIOverlay->button("Button");
    UIOverlay->text("This is a text label, and it will not wrap to new lines.");
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::Render();

    UIOverlay->updateBuffer();
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Usage: \n"
               "  <%s>   [num of windows]\n",
               argv[0]);
        return -1;
    }

    Render(atoi(argv[1])).main_loop();

    return 0;
}