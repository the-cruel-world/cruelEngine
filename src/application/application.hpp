#ifndef __cruel_engine_application__
#define __cruel_engine_application__

#include "../window/window.hpp"
#include <cassert>

namespace cruelEngine {

    struct ApplicationInfo 
    {
        std::string applicationName;
        ApplicationInfo (std::string name) : applicationName(name) {};
    };
    class Application
    {
    public: 
        Application();
        virtual ~Application();

    protected:

    private:

    };
}

#endif