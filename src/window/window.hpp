#pragma once
#include <GLFW/glfw3.h>

#include <string>

#include "../types.h"

namespace cruelEngine {
struct WindowProp {
  std::string title;
  u32 width;
  u32 height;
  bool fullscreen;

  // Give the structure init values.
  WindowProp(const std::string &_title = "Cruel Engine", u32 _width = 1280,
             u32 _height = 720, bool _fullscreen = false)
      : title(_title), width(_width), height(_height), fullscreen(_fullscreen) {
  }
};

class Window {
public:
  Window();
  Window(const WindowProp &_properties);
  ~Window();

  const bool isWindowResize() const { return onresize; }

  // whether glfw is initialized. Should be set to false mannualy
  static bool glfw_inited;

  // count how many windows you have created. Should be set to 0 mannualy.
  static u32 count;

  const WindowProp &get_properties() const { return properties; }

  GLFWwindow &get_handle() const { return *handle; }

  bool operator==(const Window &object) const {
    return handle == object.handle;
  }

private:
  bool onresize = false;

  GLFWwindow *handle = nullptr;

  WindowProp properties;

  void createWindow();
  void destroyWindow();
};

} // namespace cruelEngine