#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include <ctime>

#include "../src/window/window.hpp"
#include "../src/application/application.hpp"

#include "../src/scene/scene_header.h"
#include "../src/render/render_header.h"


u32 cruelEngine::Window::count = 0;
bool cruelEngine::Window::glfw_inited = false;

class Render : cruelEngine::Application {
public:
    Render()
    {
        std::cout << "frame time: " << frame_time << std::endl;

        render_context = std::make_unique<cruelEngine::cruelRender::RenderContext>();
        std::cout << "Render context created." << std::endl;
        cruelEngine::cruelRender::RenderProp  properties = {true, {"VK_LAYER_KHRONOS_validation"}, {}, {VK_KHR_SWAPCHAIN_EXTENSION_NAME}, false, false};
        render_context->add_session("session 1", properties);
        // render_context->add_session("session 2", properties);

        scene = std::make_unique<cruelEngine::cruelScene::Scene>(*render_context);
        std::cout << "Scene created." << std::endl;

        scene->addObject("cube");

        render_context->draw();
    }

    ~Render()
    {
        scene.reset();
        render_context.reset();
    }

    void main_loop()
    {
        clock_t past_time = clock();
        clock_t _frame_time = clock();
        while (render_context->is_context_alive())
        {
            glfwPollEvents();
            _frame_time = clock() - past_time;
            past_time = clock();
            std::cout << "frame time: " << _frame_time * 1e-3 << " ms\tusleep: " << (frame_time - _frame_time) * 1e-3 << " ms" << std::endl;
            if (_frame_time < frame_time)
                usleep(frame_time - _frame_time);
            render_context->render_frame();
        }
    }
private:

    std::unique_ptr<cruelEngine::cruelScene::Scene>             scene;
    
    std::unique_ptr<cruelEngine::cruelRender::RenderContext>    render_context;

    u32     frame_time = 16e3;
};

int main(int argc, char const *argv[])
{
    Render  render;

    render.main_loop();

    return 0;
}