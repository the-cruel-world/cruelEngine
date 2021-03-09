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

u32 cruelEngine::Window::count = 0;
bool cruelEngine::Window::glfw_inited = false;

class Render : cruelEngine::Application {
public:
  Render() {
    std::cout << "frame time: " << frame_time << std::endl;

    render_context =
        std::make_unique<cruelEngine::cruelRender::RenderContext>(context_prop);
    std::cout << "Render context created." << std::endl;

    properties.window_prop.title = "cruelEngine::Triangle::main Window";
    properties.window_prop.width = 1280;
    properties.window_prop.height = 720;
    render_context->add_session(properties.window_prop.title, properties);

    // properties.window_prop.width = 640;
    // properties.window_prop.height = 480;
    // properties.window_prop.title = "cruelEngine::Triangle::second Window";
    // render_context->add_session(properties.window_prop.title, properties);

    scene = std::make_unique<cruelEngine::cruelScene::Scene>(*render_context);
    std::cout << "Scene created." << std::endl;

    // scene->addObject("cube", 0,
    //                  cruelEngine::cruelScene::SCENE_OBJ_TYPE_TRIANGLE);

    // scene->addObject("tri", 1,
    //                  cruelEngine::cruelScene::SCENE_OBJ_TYPE_TRIANGLE);

    scene->addObject("sphere", 0,
                     cruelEngine::cruelScene::SCENE_OBJ_TYPE_SPHERE);

    render_context->draw();
  }

  ~Render() {
    scene.reset();
    render_context.reset();
  }

  void main_loop() {
    clock_t past_time = clock();
    clock_t _frame_time = clock();
    while (render_context->is_context_alive()) {
      glfwPollEvents();
      _frame_time = clock() - past_time;
      past_time = clock();
      // std::cout << "frame time: " << _frame_time * 1e-3 << " ms\tusleep: " <<
      // (frame_time - _frame_time) * 1e-3 << " ms" << std::endl;
      if (_frame_time < frame_time)
        usleep(frame_time - _frame_time);
      render_context->render_frame();
    }
  }

private:
  cruelEngine::cruelRender::RenderProp context_prop = {
      .appInfo = {.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                  .pApplicationName = "Cruel Engine",
                  .pEngineName = "Cruel Engine",
                  .apiVersion = USED_VULKAN_API_VERSION},
      .validation = true,
      .validationLayers = {"VK_LAYER_KHRONOS_validation"},
      .enabledInstanceExtensions = {},
      .enabledDeviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
      .features = {.fillModeNonSolid = true, .wideLines = true},
      .flags = {},
      .vsync = false,
      .overlay = false,
  };
  cruelEngine::cruelRender::SessionProp properties = {
      {"cruelEngine::Triangle::main Window", 720, 640, false}};

  std::unique_ptr<cruelEngine::cruelScene::Scene> scene;

  std::unique_ptr<cruelEngine::cruelRender::RenderContext> render_context;

  u32 frame_time = 16e3;
};

int main(int argc, char const *argv[]) {
  Render render;

  render.main_loop();

  return 0;
}