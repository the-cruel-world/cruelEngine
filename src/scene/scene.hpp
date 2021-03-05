#pragma once
#include "../common.h"
#include "object.hpp"
#include "../render/render_header.h"

namespace cruelEngine {
    class RenderContext;
namespace cruelRender {
    class RenderContext;
}

namespace cruelScene {
    class Object;

    class Scene {
    public:
        Scene (cruelRender::RenderContext &render_context);
        virtual ~Scene ();

        void addObject(std::string name, u32 session_idx, ObjectType type);

        void rmObject(std::string name);

        const std::vector<std::unique_ptr<Object>> &get_objs() const {return sceneObjects;}

        void update();

        void render();

    protected:
        
        std::vector<std::unique_ptr<Object>> sceneObjects {};

        cruelRender::RenderContext   &render_context;
    };
}
}