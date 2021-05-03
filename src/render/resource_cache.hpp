#pragma once
#include "hash.h"
#include "vkcommon.h"

#include "render/vulkan/buffer.hpp"
#include "render/vulkan/descriptor.hpp"
#include "render/vulkan/frame_buffer.hpp"
#include "render/vulkan/pipeline.hpp"
#include "render/vulkan/render_pass.hpp"
#include "render/vulkan/shader.hpp"

namespace cruelEngine::cruelRender
{
class ShaderModule;
class Pipeline;
class PipelineLayout;
class GraphicsPipeline;
class ComputePipeline;
class DescriptorSet;
class DescriptorSetLayout;
class DescriptorPool;
class RenderPass;
class FrameBuffer;

class LogicalDevice;

struct LoadStoreInfo;
struct Attachment;

class RenderTarget;
class RenderPipline;
class Subpass;

/**
 * \brief Resource cache. It manages the resources that creates on gpu includes
 * framebuffer, buffer, pipeline, renderpass, shadermodule, etc.
 */
class ResourceCache
{
public:
    ResourceCache(LogicalDevice &device);

    ResourceCache(const ResourceCache &) = delete;

    ResourceCache(ResourceCache &&) = delete;

    ResourceCache &operator=(const ResourceCache &) = delete;

    ResourceCache &operator=(ResourceCache &&) = delete;

    ~ResourceCache();

public:
    ShaderModule &request_shader_module();

    PipelineLayout &request_pipeline_layout();

    GraphicsPipeline &request_graphics_pipeline(PipelineStatus &pipeline_state);

    ComputePipeline &request_compute_pipeline(PipelineStatus &pipeline_state);

    DescriptorPool &request_descriptor_pool(const VkDescriptorPoolSize &pool_size, u32 maxSets);

    DescriptorSetLayout &request_descriptor_set_layout(
        const std::vector<VkDescriptorSetLayoutBinding> &new_bindings);

    DescriptorSet &request_descriptor_set(DescriptorSetLayout &layout, DescriptorPool &pool);

    RenderPass &request_render_pass(const std::vector<VkAttachmentDescription> &attachments,
                                    const std::vector<SubpassInfo> &            subpasses);

    RenderPass &request_render_pass(const std::vector<Attachment> &   attachments,
                                    const std::vector<LoadStoreInfo> &load_store,
                                    const std::vector<LoadStoreInfo> &stencil_load_store,
                                    const std::vector<SubpassInfo> &  subpasses);

    FrameBuffer &request_framebuffer(const std::vector<VkImageView> &imageView,
                                     const VkExtent2D &extent, const RenderPass &renderPass);

    void warmup();

    void set_pipeline_cache(VkPipelineCache pipeline_cache);

    void update_descriptor_set();

    void clear_pipeline();

    void clear_frame_buffer();

    void clear_shader_module();

    void clear();

private:
    LogicalDevice &device;

    VkPipelineCache pipeline_cache = VK_NULL_HANDLE;

    //! The first value is the hash value of elements
    /// Hash map
    std::unordered_map<std::size_t, ShaderModule> shader_modules;

    std::unordered_map<std::size_t, PipelineLayout> pipeline_layouts;

    std::unordered_map<std::size_t, DescriptorPool> descriptor_pools;

    std::unordered_map<std::size_t, DescriptorSetLayout> descriptor_set_layouts;

    std::unordered_map<std::size_t, DescriptorSet> descriptor_sets;

    std::unordered_map<std::size_t, RenderPass> render_passes;

    std::unordered_map<std::size_t, GraphicsPipeline> graphics_pipelines;

    std::unordered_map<std::size_t, ComputePipeline> compute_pipelines;

    std::unordered_map<std::size_t, FrameBuffer> framebuffers;

    // mutex
    std::mutex pipeline_cache_mutex;

    std::mutex descriptor_pool_mutex;

    std::mutex descriptor_set_layout_mutex;

    std::mutex descriptor_set_mutex;

    std::mutex pipeline_layout_mutex;

    std::mutex shader_module_mutex;

    std::mutex graphics_pipeline_mutex;

    std::mutex render_pass_mutex;

    std::mutex compute_pipeline_mutex;

    std::mutex framebuffer_mutex;

    template <class T, class... A>
    T &request_resource(std::mutex &resource_mutex, std::unordered_map<std::size_t, T> &resources,
                        A &...args)
    {
        std::lock_guard<std::mutex> guard(resource_mutex);

        size_t hash_val{0U};
        hash_param(hash_val, args...);

        auto res_it = resources.find(hash_val);

        if (res_it != resources.end())
        {
            return res_it->second;
        }

        T resource(device, args...);

        auto res_ins_it = resources.template emplace(hash_val, std::move(resource));

        res_it = res_ins_it.first;

        return res_it->second;
    }
};
} // namespace cruelEngine::cruelRender