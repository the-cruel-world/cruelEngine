#include "window.hpp"

namespace realEngine {
    Window::Window() : property(WindowProp())
    {
        createWindow();
    }
    Window::Window(const WindowProp& _property) : property (_property)
    {
        createWindow();
    }

    void Window::createWindow(){
        // If the glfw system is not inited, init it.
        if (!glfw_inited)
            glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // Create different type of window fullscreen or not.
        if (property.fullscreen)
            window = glfwCreateWindow(property.width, property.height, property.title.c_str(), glfwGetPrimaryMonitor(), nullptr);
        else 
            window = glfwCreateWindow(property.width, property.height, property.title.c_str(), nullptr, nullptr);
        if (window)
            count ++;
    }

    void Window::destroyWindow()
    {
        // if window is not not created, then create one
        if (window) {
            glfwDestroyWindow(window);
        } else return;
        // if a new window is created, count += 1 
        if (!window)
            count --;
    }

    Window::~Window(){
        destroyWindow();
        // If this is the last window and glfw is still running, terminate it.
        if (count == 0)
            if (glfw_inited)
                glfwTerminate();
    }
}