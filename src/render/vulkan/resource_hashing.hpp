#pragma once
#include "../vkcommon.h"
#include <glm/gtx/hash.hpp>

#include "descriptor.hpp"
#include "pipeline.hpp"
#include "render_pass.hpp"

namespace cruelEngine::cruelRender
{
template <class T>
inline void hash_combine(size_t &seed, const T &arg)
{
    std::hash<T> hasher;
    glm::detail::hash_combine(seed, hasher(arg));
}

template <typename T>
inline void hash_param(size_t &seed, const T &value)
{
    hash_combine(seed, value);
}

} // namespace cruelEngine::cruelRender

namespace std
{
template <>
struct hash<cruelEngine::cruelRender::DescriptorPool>
{
    std::size_t operator()(const cruelEngine::cruelRender::DescriptorPool &descriptor_pool) const
    {
        std::size_t result = 0;

        cruelEngine::cruelRender::hash_combine(result, descriptor_pool.getPoolSize());

        return result;
    }
};

template <>
struct hash<cruelEngine::cruelRender::DescriptorSetLayout>
{
    std::size_t
        operator()(const cruelEngine::cruelRender::DescriptorSetLayout &descriptor_set_layout) const
    {
        std::size_t result = 0;

        cruelEngine::cruelRender::hash_combine(result, descriptor_set_layout.get_handle());

        return result;
    }
};

template <>
struct hash<cruelEngine::cruelRender::PipelineLayout>
{
    std::size_t operator()(const cruelEngine::cruelRender::PipelineLayout &pipeline_layout) const
    {
        std::size_t result = 0;

        cruelEngine::cruelRender::hash_combine(result, pipeline_layout.get_handle());

        return result;
    }
};

template <>
struct hash<cruelEngine::cruelRender::RenderPass>
{
    std::size_t operator()(const cruelEngine::cruelRender::RenderPass &render_pass) const
    {
        std::size_t result = 0;

        cruelEngine::cruelRender::hash_combine(result, render_pass.get_handle());

        return result;
    }
};

template <>
struct hash<VkVertexInputAttributeDescription>
{
    std::size_t operator()(const VkVertexInputAttributeDescription &vertex_attrib) const
    {
        std::size_t result = 0;

        cruelEngine::cruelRender::hash_combine(result, vertex_attrib.binding);
        cruelEngine::cruelRender::hash_combine(
            result, static_cast<std::underlying_type<VkFormat>::type>(vertex_attrib.format));
        cruelEngine::cruelRender::hash_combine(result, vertex_attrib.location);
        cruelEngine::cruelRender::hash_combine(result, vertex_attrib.offset);

        return result;
    }
};

} // namespace std