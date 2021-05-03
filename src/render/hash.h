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
} // namespace cruelEngine::cruelRender

namespace std
{
/** To be continue... */
template <>
struct hash<cruelEngine::cruelRender::PipelineStatus>
{
    std::size_t operator()(const cruelEngine::cruelRender::PipelineStatus &pipeline_state) const
    {
        std::size_t res = 0;
        {
            cruelEngine::cruelRender::hash_combine(res, pipeline_state);
        }
        return res;
    }
};

template <>
struct hash<VkExtent2D>
{
    std::size_t operator()(const VkExtent2D &extent) const
    {
        std::size_t res = 0;
        cruelEngine::cruelRender::hash_combine(res, extent.width);
        cruelEngine::cruelRender::hash_combine(res, extent.height);
        return res;
    }
};

template <>
struct hash<VkExtent3D>
{
    std::size_t operator()(const VkExtent3D &extent) const
    {
        std::size_t res = 0;
        cruelEngine::cruelRender::hash_combine(res, extent.width);
        cruelEngine::cruelRender::hash_combine(res, extent.height);
        cruelEngine::cruelRender::hash_combine(res, extent.depth);
        return res;
    }
};

template <>
struct hash<VkDescriptorPoolSize>
{
    std::size_t operator()(const VkDescriptorPoolSize &poolSize) const
    {
        std::size_t res = 0;
        cruelEngine::cruelRender::hash_combine(res, poolSize.type);
        cruelEngine::cruelRender::hash_combine(res, poolSize.descriptorCount);
        return res;
    }
};

template <>
struct hash<cruelEngine::cruelRender::RenderPass>
{
    std::size_t operator()(const cruelEngine::cruelRender::RenderPass &renderPass) const
    {
        std::size_t res = 0;
        cruelEngine::cruelRender::hash_combine(res, renderPass.get_handle());
        return res;
    }
};

template <>
struct hash<cruelEngine::cruelRender::DescriptorSetLayout>
{
    std::size_t operator()(const cruelEngine::cruelRender::DescriptorSetLayout &layout) const
    {
        std::size_t res = 0;
        cruelEngine::cruelRender::hash_combine(res, layout.get_handle());
        return res;
    }
};

template <>
struct hash<cruelEngine::cruelRender::DescriptorPool>
{
    std::size_t operator()(const cruelEngine::cruelRender::DescriptorPool &pool) const
    {
        std::size_t res = 0;
        cruelEngine::cruelRender::hash_combine(res, pool.get_handle());
        return res;
    }
};

template <>
struct hash<VkDescriptorSetLayoutBinding>
{
    std::size_t operator()(const VkDescriptorSetLayoutBinding &binding) const
    {
        std::size_t res = 0;
        cruelEngine::cruelRender::hash_combine(res, binding.binding);
        cruelEngine::cruelRender::hash_combine(res, binding.descriptorType);
        cruelEngine::cruelRender::hash_combine(res, binding.descriptorCount);
        cruelEngine::cruelRender::hash_combine(res, binding.stageFlags);
        cruelEngine::cruelRender::hash_combine(res, binding.pImmutableSamplers);

        return res;
    }
};

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

template <>
struct hash<VkAttachmentDescription>
{
    std::size_t operator()(const VkAttachmentDescription &attachment) const
    {
        std::size_t res = 0;
        cruelEngine::cruelRender::hash_combine(res, attachment.format);
        cruelEngine::cruelRender::hash_combine(res, attachment.samples);
        cruelEngine::cruelRender::hash_combine(res, attachment.loadOp);
        cruelEngine::cruelRender::hash_combine(res, attachment.storeOp);
        cruelEngine::cruelRender::hash_combine(res, attachment.stencilLoadOp);
        cruelEngine::cruelRender::hash_combine(res, attachment.stencilStoreOp);
        cruelEngine::cruelRender::hash_combine(res, attachment.initialLayout);
        cruelEngine::cruelRender::hash_combine(res, attachment.finalLayout);

        return res;
    }
};

template <>
struct hash<cruelEngine::cruelRender::SubpassInfo>
{
    std::size_t operator()(const cruelEngine::cruelRender::SubpassInfo &subpassInfo) const
    {
        std::size_t res = 0;
        for (auto &input : subpassInfo.input_attachments)
        {
            cruelEngine::cruelRender::hash_combine(res, input);
        }
        for (auto &output : subpassInfo.output_attachments)
        {
            cruelEngine::cruelRender::hash_combine(res, output);
        }
        for (auto &color : subpassInfo.color_resolve_attachments)
        {
            cruelEngine::cruelRender::hash_combine(res, color);
        }
        cruelEngine::cruelRender::hash_combine(res, subpassInfo.disable_depth_stencil_attachment);
        cruelEngine::cruelRender::hash_combine(res, subpassInfo.depth_stencil_resolve_attachment);
        cruelEngine::cruelRender::hash_combine(res, subpassInfo.depth_stencil_resolve_mode);

        return res;
    }
};
} // namespace std

namespace cruelEngine::cruelRender
{
template <typename T>
inline void hash_param(size_t &seed, const T &value)
{
    hash_combine(seed, value);
}

template <>
inline void hash_param<std::vector<VkImageView>>(size_t &                        seed,
                                                 const std::vector<VkImageView> &value)
{
    for (auto &imgView : value)
    {
        hash_combine(seed, imgView);
    }
}

template <>
inline void hash_param<std::vector<VkDescriptorSetLayoutBinding>>(
    size_t &seed, const std::vector<VkDescriptorSetLayoutBinding> &value)
{
    for (auto &binding : value)
    {
        hash_combine(seed, binding);
    }
}

template <>
inline void hash_param<std::vector<LoadStoreInfo>>(size_t &                          seed,
                                                   const std::vector<LoadStoreInfo> &value)
{
    for (auto &load_store_info : value)
    {
        hash_combine(seed, load_store_info);
    }
}

template <>
inline void hash_param<std::vector<Attachment>>(size_t &seed, const std::vector<Attachment> &value)
{
    for (auto &attach : value)
    {
        hash_combine(seed, attach);
    }
}

template <>
inline void hash_param<std::vector<VkAttachmentDescription>>(
    size_t &seed, const std::vector<VkAttachmentDescription> &value)
{
    for (auto &attach : value)
    {
        hash_combine(seed, attach);
    }
}

template <>
inline void hash_param<std::vector<SubpassInfo>>(size_t &                        seed,
                                                 const std::vector<SubpassInfo> &value)
{
    for (auto &subpass : value)
    {
        hash_combine(seed, subpass);
    }
}

template <typename T, typename... Args>
inline void hash_param(size_t &seed, const T &first_arg, const Args &...args)
{
    hash_param(seed, first_arg);

    hash_param(seed, args...);
}

} // namespace cruelEngine::cruelRender

#endif