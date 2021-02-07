#include <iostream>

//#include "../src/render/engine.hpp"
#include "../src/window/window.hpp"
#include "../src/application/application.hpp"
#include <vector>
#include <algorithm>

u32 realEngine::Window::count = 0;
bool realEngine::Window::glfw_inited = false;


int main(int argc, char const *argv[])
{
    // Engine app;
    
    // try {
    //     app.run();
    // } catch (const std::runtime_error &e) {
    //     std::cerr << e.what() << std::endl;
    //     return -1;
    // }

    // realEngine::WindowProp property;

    // property.title = "Hello Test";

    // realEngine::Window window;

    // if (window.window)
    //     std::cout << "Created successfully!" << std::endl;
    // else return -1;

    // while (!glfwWindowShouldClose(window.window)){
    //     glfwPollEvents();
    // }

    realEngine::Application app;
    
    return 0;
}