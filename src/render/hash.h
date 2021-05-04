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
template <>
struct hash<cruelEngine::cruelRender::ShaderModule>
{
    std::size_t operator()(const cruelEngine::cruelRender::ShaderModule &shader) const
    {
        std::size_t res = 0;
        for (auto &a : shader.get_source())
        {
            cruelEngine::cruelRender::hash_combine(res, a);
        }
        cruelEngine::cruelRender::hash_combine(res, shader.get_entry_point());
        cruelEngine::cruelRender::hash_combine(res, shader.get_shader_stage());

        return res;
    }
};

template <>
struct hash<VkPipelineLayoutCreateInfo>
{
    std::size_t operator()(const VkPipelineLayoutCreateInfo &pipelineLayoutCI) const
    {
        std::size_t res = 0;
        cruelEngine::cruelRender::hash_combine(res, pipelineLayoutCI.setLayoutCount);
        cruelEngine::cruelRender::hash_combine(res, pipelineLayoutCI.pSetLayouts);
        cruelEngine::cruelRender::hash_combine(res, pipelineLayoutCI.pushConstantRangeCount);
        cruelEngine::cruelRender::hash_combine(res, pipelineLayoutCI.pPushConstantRanges);
        return res;
    }
};

template <>
struct hash<cruelEngine::cruelRender::PipelineStatus::StencilOpState>
{
    std::size_t
        operator()(const cruelEngine::cruelRender::PipelineStatus::StencilOpState &opstate) const
    {
        std::size_t res = 0;
        cruelEngine::cruelRender::hash_combine(res, opstate.fail_op);
        cruelEngine::cruelRender::hash_combine(res, opstate.pass_op);
        cruelEngine::cruelRender::hash_combine(res, opstate.depth_fail_op);
        cruelEngine::cruelRender::hash_combine(res, opstate.compare_op);
        return res;
    }
};

template <>
struct hash<VkVertexInputBindingDescription>
{
    std::size_t operator()(const VkVertexInputBindingDescription &bindings) const
    {
        std::size_t res = 0;
        cruelEngine::cruelRender::hash_combine(res, bindings.binding);
        cruelEngine::cruelRender::hash_combine(res, bindings.stride);
        cruelEngine::cruelRender::hash_combine(res, bindings.inputRate);
        return res;
    }
};

template <>
struct hash<VkVertexInputAttributeDescription>
{
    std::size_t operator()(const VkVertexInputAttributeDescription &attributes) const
    {
        std::size_t res = 0;
        cruelEngine::cruelRender::hash_combine(res, attributes.location);
        cruelEngine::cruelRender::hash_combine(res, attributes.binding);
        cruelEngine::cruelRender::hash_combine(res, attributes.format);
        cruelEngine::cruelRender::hash_combine(res, attributes.offset);

        return res;
    }
};

template <>
struct hash<cruelEngine::cruelRender::PipelineStatus::ColorBlendAttachmentState>
{
    std::size_t operator()(const cruelEngine::cruelRender::PipelineStatus::ColorBlendAttachmentState
                               &attachments) const
    {
        std::size_t res = 0;
        cruelEngine::cruelRender::hash_combine(res, attachments.blend_enable);
        cruelEngine::cruelRender::hash_combine(res, attachments.src_color_blend_factor);
        cruelEngine::cruelRender::hash_combine(res, attachments.dst_color_blend_factor);
        cruelEngine::cruelRender::hash_combine(res, attachments.color_blend_op);
        cruelEngine::cruelRender::hash_combine(res, attachments.src_alpha_blend_factor);
        cruelEngine::cruelRender::hash_combine(res, attachments.dst_alpha_blend_factor);
        cruelEngine::cruelRender::hash_combine(res, attachments.alpha_blend_op);
        cruelEngine::cruelRender::hash_combine(res, attachments.color_write_mask);
        return res;
    }
};

template <>
struct hash<cruelEngine::cruelRender::PipelineStatus>
{
    std::size_t operator()(const cruelEngine::cruelRender::PipelineStatus &pipeline_state) const
    {
        std::size_t res = 0;
        {
            cruelEngine::cruelRender::hash_combine(res, pipeline_state.get_pipeline_layout());
            cruelEngine::cruelRender::hash_combine(res, pipeline_state.get_render_pass());
        }
        {
            for (auto &binding : pipeline_state.get_vertex_input_state().bindings)
            {
                cruelEngine::cruelRender::hash_combine(res, binding);
            }
            for (auto &attribute : pipeline_state.get_vertex_input_state().attributes)
            {
                cruelEngine::cruelRender::hash_combine(res, attribute);
            }
        }
        {
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_input_assembly_state().primitive_restart_enable);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_input_assembly_state().topology);
        }
        {
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_rasterization_state().depth_clamp_enable);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_rasterization_state().rasterizer_discard_enable);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_rasterization_state().polygon_mode);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_rasterization_state().cull_mode);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_rasterization_state().front_face);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_rasterization_state().depth_bias_enable);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_rasterization_state().depth_bias_clamp);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_rasterization_state().depth_bias_slope_factor);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_rasterization_state().line_width);
        }
        {
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_viewport_state().viewport_count);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_viewport_state().scissor_count);

            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_multisample_state().rasterization_samples);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_multisample_state().sample_shading_enable);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_multisample_state().min_sample_shading);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_multisample_state().sample_mask);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_multisample_state().alpha_to_coverage_enable);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_multisample_state().alpha_to_one_enable);

            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_depth_stencil_state().depth_test_enable);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_depth_stencil_state().depth_write_enable);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_depth_stencil_state().depth_compare_op);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_depth_stencil_state().depth_bounds_test_enable);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_depth_stencil_state().stencil_test_enable);
            cruelEngine::cruelRender::hash_combine(res,
                                                   pipeline_state.get_depth_stencil_state().front);
            cruelEngine::cruelRender::hash_combine(res,
                                                   pipeline_state.get_depth_stencil_state().back);

            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_color_blend_state().logic_op_enable);
            cruelEngine::cruelRender::hash_combine(res,
                                                   pipeline_state.get_color_blend_state().logic_op);
            for (auto const &attachments : pipeline_state.get_color_blend_state().attachments)
            {
                cruelEngine::cruelRender::hash_combine(res, attachments);
            }
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_color_blend_state().blend_constants[0]);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_color_blend_state().blend_constants[1]);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_color_blend_state().blend_constants[2]);
            cruelEngine::cruelRender::hash_combine(
                res, pipeline_state.get_color_blend_state().blend_constants[3]);

            for (auto &dynamic_state : pipeline_state.get_dynamic_state().dynamic_states)
            {
                cruelEngine::cruelRender::hash_combine(res, dynamic_state);
            }

            cruelEngine::cruelRender::hash_combine(res, pipeline_state.get_subpass_index());
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
inline void hash_param<std::vector<ShaderModule>>(size_t &                         seed,
                                                  const std::vector<ShaderModule> &value)
{
    for (auto &shader : value)
    {
        hash_combine(seed, shader);
    }
}

template <>
inline void hash_param<std::vector<VkDynamicState>>(size_t &                           seed,
                                                    const std::vector<VkDynamicState> &value)
{
    for (auto &dynamic_states : value)
    {
        hash_combine(seed, dynamic_states);
    }
}

template <>
inline void hash_param<std::vector<VkVertexInputBindingDescription>>(
    size_t &seed, const std::vector<VkVertexInputBindingDescription> &value)
{
    for (auto &binding : value)
    {
        hash_combine(seed, binding);
    }
}

template <>
inline void hash_param<std::vector<VkVertexInputAttributeDescription>>(
    size_t &seed, const std::vector<VkVertexInputAttributeDescription> &value)
{
    for (auto &attribute : value)
    {
        hash_combine(seed, attribute);
    }
}

template <>
inline void hash_param<std::vector<PipelineStatus::ColorBlendAttachmentState>>(
    size_t &seed, const std::vector<PipelineStatus::ColorBlendAttachmentState> &value)
{
    for (auto &attachment : value)
    {
        hash_combine(seed, attachment);
    }
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