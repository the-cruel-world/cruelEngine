#include <algorithm>
#include <ctime>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <vector>

// #include <GLFW/glfw3.h>
#include <../src/window/window.hpp>

u32 cruelEngine::Window::count = 0;
bool cruelEngine::Window::glfw_inited = false;

int main(int argc, char const *argv[]) {

  std::vector<std::unique_ptr<cruelEngine::Window>> windows{};
  windows.push_back(std::make_unique<cruelEngine::Window>());
  windows.push_back(std::make_unique<cruelEngine::Window>());
  windows.push_back(std::make_unique<cruelEngine::Window>());

  while (windows.size() > 0) {
    glfwPollEvents();
    windows.erase(
        std::remove_if(windows.begin(), windows.end(),
                       [](std::unique_ptr<cruelEngine::Window> const &window) {
                         return glfwWindowShouldClose(&window->get_handle());
                       }),
        windows.end());
    usleep(3e5);
  }

  return 0;
}