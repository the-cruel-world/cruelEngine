#pragma once

#include "../common.h"
#include "../debug/debug.hpp"


namespace cruelEngine {
    class Window;

// namespace cruelScene {
//     class Scene;
//     class Object;
// }

    class RenderContext {
    public:
        RenderContext(const Window& _window);

        virtual ~RenderContext();

    public:
        // virtual void        loadScene(const cruelEngine::cruelScene::Scene &theScene) = 0;
        // virtual void        loadObject(const cruelEngine::cruelScene::Object &theObject) = 0;

    protected:
        const Window&           mainWindow;
        // std::unique_ptr<cruelScene::Scene> scene;
    };
}