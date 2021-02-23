#pragma once

#include "../common.h"

#include "object.hpp"

namespace cruelEngine {
namespace cruelScene {
    class Scene {
    public:
        Scene () {}
        virtual ~Scene () {}

        void addObject(const Object& object);

        void rmObject(const char *objectName);

        virtual void onUpdate();

        void        newFrame();

    private:
        std::vector<Object> sceneObjects;
    };
}
}