#ifndef __cruel_engine_application__
#define __cruel_engine_application__

#include <cassert>

#include "../window/window.hpp"

namespace cruelEngine {

struct ApplicationInfo {
  std::string applicationName;
  ApplicationInfo(std::string name) : applicationName(name){};
};
class Application {
public:
  Application();
  virtual ~Application();

protected:
private:
};
} // namespace cruelEngine

#endif