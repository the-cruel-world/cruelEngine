#include <algorithm>
#include <ctime>
#include <iostream>
#include <memory>
#include <unistd.h>
#include <vector>
#include <typeinfo>
#include <typeindex>

// #include <GLFW/glfw3.h>
#include <../src/window/window.hpp>

u32 cruelEngine::Window::count = 0;
bool cruelEngine::Window::glfw_inited = false;

class test {

};

int main(int argc, char const *argv[])
{
  test newTest;
  int a = 0;
  std::type_index type(typeid(a));
  std::cout << type.name() << std::endl;
  return 0;
}