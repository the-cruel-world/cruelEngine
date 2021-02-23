#ifndef __cruel_engine_window__
#define __cruel_engine_window__

#include <string>

// If Vulkan is the backend of render
#ifdef __VULKAN__
#define GLFW_INCLUDE_VULKAN
#endif

// For wayland protocol
#ifdef __WAYLAND__

#endif 

#include <GLFW/glfw3.h>
#include "../types.h"

namespace cruelEngine{
    struct WindowProp {
        std::string title;
        u32 width;
        u32 height;
        bool fullscreen;
        
        // Give the structure init values.
        WindowProp(const std::string& _title = "Cruel Engine",
            u32 _width = 1280, u32 _height = 720, bool _fullscreen = false) 
            : title(_title), width(_width), height(_height), fullscreen(_fullscreen)
        {}
    };

    class Window 
    {
    public:
        // whether glfw is initialized. Should be set to false mannualy
        static bool glfw_inited;

        // count how many windows you have created. Should be set to 0 mannualy.
        static u32 count;

        bool        onresize = false;

        GLFWwindow * window = nullptr;
        WindowProp property;
        
        Window();
        Window(const WindowProp& _property);
        virtual ~Window();

        const bool      isWindowResize() const {return onresize;}

        virtual void createWindow();
        virtual void destroyWindow();
    };

}
#endif
