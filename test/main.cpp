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

struct numptr
{
  std::vector<std::shared_ptr<int>> number{};

  void load_number(std::shared_ptr<int> new_num)
  {
    number.push_back(std::move(new_num));
  }
  ~numptr() { number.clear(); }
};

int main(int argc, char const *argv[])
{

  auto hello = std::make_shared<int>(1);
  std::cout << "Ref Count: " << hello.use_count() << std::endl;
  {
    numptr temp;
    temp.load_number(hello);
  std::cout << "Ref Count: " << hello.use_count() << std::endl;
    temp.load_number(hello);
  std::cout << "Ref Count: " << hello.use_count() << std::endl;
    temp.load_number(hello);
  std::cout << "Ref Count: " << hello.use_count() << std::endl;
    temp.load_number(hello);
  std::cout << "Ref Count: " << hello.use_count() << std::endl;
  }
  std::cout << "Ref Count: " << hello.use_count() << std::endl;


  return 0;
}