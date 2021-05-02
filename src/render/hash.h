#ifndef CRUELENGINE_HASH_H
#define CRUELENGINE_HASH_H
#pragma once
#include "vkcommon.h"
#include <glm/gtx/hash.hpp>

#include "render/render_target.hpp"
#include "render/vulkan/buffer.hpp"
#include "render/vulkan/descriptor.hpp"
#include "render/vulkan/frame_buffer.hpp"
#include "render/vulkan/pipeline.hpp"
#include "render/vulkan/render_pass.hpp"
#include "render/vulkan/shader.hpp"


namespace cruelEngine::cruelRender
{
template <class T>
inline void hash_combine(size_t &seed, const T &v)
{
    std::hash<T> hasher;
    glm::detail::hash_combine(seed, hasher(v));
};
}

namespace std
{
template <>
struct hash<cruelEngine::cruelRender::LoadStoreInfo>
{
    std::size_t operator()(const cruelEngine::cruelRender::LoadStoreInfo &load_store) const
    {
        std::size_t res = 0;
        cruelEngine::cruelRender::hash_combine(res, load_store.loadOp);
        cruelEngine::cruelRender::hash_combine(res, load_store.storeOp);

        return res;
    }
};

template <>
struct hash<cruelEngine::cruelRender::Attachment>
{
    std::size_t operator()(const cruelEngine::cruelRender::Attachment &attachment) const
    {
        std::size_t res = 0;
        cruelEngine::cruelRender::hash_combine(res, attachment.format);
        cruelEngine::cruelRender::hash_combine(res, attachment.samples);
        cruelEngine::cruelRender::hash_combine(res, attachment.usage);
        cruelEngine::cruelRender::hash_combine(res, attachment.initLayout);
        cruelEngine::cruelRender::hash_combine(res, attachment.finalLayout);

        return res;
    }
};
}

namespace cruelEngine::cruelRender
{

template <typename T>
inline void hash_param(size_t &seed, const T &value)
{
    hash_combine(seed, value);
}

template <>
inline void hash_param<std::vector<LoadStoreInfo>>(
    size_t &                          seed,
    const std::vector<LoadStoreInfo> &value)
{
    for (auto &load_store_info : value)
    {
        hash_combine(seed, load_store_info);
    }
}

} // namespace cruelEngine::cruelRender

#endif