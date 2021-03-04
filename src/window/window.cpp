#include "window.hpp"

namespace cruelEngine {
    Window::Window() : properties(WindowProp())
    {
        createWindow();
    }
    Window::Window(const WindowProp& _property) : properties (_property)
    {
        createWindow();
    }

    void Window::createWindow(){
        // If the glfw system is not inited, init it.
        if (!glfw_inited)
            glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        // glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // Create different type of window fullscreen or not.
        if (properties.fullscreen)
            handle = glfwCreateWindow(properties.width, properties.height, properties.title.c_str(), glfwGetPrimaryMonitor(), nullptr);
        else 
            handle = glfwCreateWindow(properties.width, properties.height, properties.title.c_str(), nullptr, nullptr);

        // glfwSetWindowUserPointer(window, this);
        
        if (handle)
            count ++;
    }

    void Window::destroyWindow()
    {
        // if window is not not created, then create one
        if (handle) {
            glfwDestroyWindow(handle);
        } else return;
        // if a new window is created, count += 1 
        if (!handle)
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