#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <ctime>

#include "../src/window/window.hpp"
#include "../src/application/application.hpp"

#include "../src/scene/scene_header.h"
#include "../src/render/vulkan/vkheader.h"


u32 cruelEngine::Window::count = 0;
bool cruelEngine::Window::glfw_inited = false;

class Render : cruelEngine::Application {
public:
    Render()
    {
        std::cout << "frame time: " << frame_time << std::endl;
        main_window = std::make_unique<cruelEngine::Window>(window_prop);
        std::cout << "Window created." << std::endl;
        render_context = std::make_unique<cruelEngine::VulkanContext::VulkanContext>(*main_window);
        std::cout << "Render context created." << std::endl;
        scene = std::make_unique<cruelEngine::cruelScene::Scene>(*render_context);
        std::cout << "Scene created." << std::endl;

        cruelEngine::VulkanContext::DriverVersion version = render_context->get_gpu().get_driver_version();
        std::cout << "Gpu driver version: " << version.major << "." << version.minor << "." << version.patch << std::endl;

        scene->addObject("Cube");

        cruelEngine::VulkanContext::RenderTask *task = scene->get_obj()[0]->get_task();
        if (task->get_status() == cruelEngine::VulkanContext::RenderTask::RENDER_TASK_UNINIRED)
            std::cout << "Task not inited." << std::endl;

        render_context->draw();
    }

    ~Render()
    {
        render_context.reset();
        scene.reset();
        main_window.reset();
    }

    void main_loop()
    {
        if (main_window){
            clock_t past_time = clock();
            clock_t _frame_time = clock();
            while (!glfwWindowShouldClose(&main_window->get_handle())){
                glfwPollEvents();
                _frame_time = clock() - past_time;
                past_time = clock();
                // std::cout << "frame time: " << _frame_time * 1e-3 << " ms\tusleep: " << (frame_time - _frame_time) * 1e-3 << " ms" << std::endl;
                if (_frame_time < frame_time)
                    usleep(frame_time - _frame_time);
                render_context->render_frame();
            }
        }
    }
private:

    cruelEngine::WindowProp                  window_prop = {"Render", 1280, 720, false};

    std::unique_ptr<cruelEngine::Window>     main_window;
    std::unique_ptr<cruelEngine::cruelScene::Scene> scene;
    std::unique_ptr<cruelEngine::VulkanContext::VulkanContext> render_context;

    u32     frame_time = 16e3;
};

int main(int argc, char const *argv[])
{
    Render  render;

    render.main_loop();

    return 0;
}