//
// Created by yiwen on 4/12/21.
//

#ifndef CRUELENGINE_GLTFLAODER_HPP
#define CRUELENGINE_GLTFLAODER_HPP

#pragma once
#include "scene/scene_header.h"

namespace cruelEngine::cruelScene
{
class GltfLaoder
{
    struct BufferView
    {
        uint32_t buffer_index;
        uint32_t offset;
        uint32_t length;
        uint32_t stride;
    };

    struct Accessor
    {
        uint32_t view;
        uint32_t offset;
        uint32_t count;
        uint32_t stride;

        uint32_t components;

        union
        {
            float    f32;
            uint32_t u32;
            int32_t  i32;
        } min[16], max[16];
    };
};
} // namespace cruelEngine::cruelScene
#endif // CRUELENGINE_GLTFLAODER_HPP
