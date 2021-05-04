#include "resource_cache.hpp"
#include "hash.h"
#include "render/vulkan/logical_device.hpp"

namespace cruelEngine::cruelRender
{
ResourceCache::ResourceCache(LogicalDevice &device) : device{device}
{
    CRUEL_LOG("%s\n", "ResourceCache created.");
}

void ResourceCache::clear_pipeline()
{
    compute_pipelines.clear();
    graphics_pipelines.clear();
}

void ResourceCache::clear_frame_buffer()
{
    framebuffers.clear();
}

void ResourceCache::clear_shader_module()
{
    shader_modules.clear();
}

ResourceCache::~ResourceCache()
{
    pipeline_layouts.clear();
    descriptor_sets.clear();
    descriptor_set_layouts.clear();
    render_passes.clear();
    clear_shader_module();
    clear_pipeline();
    clear_frame_buffer();
    CRUEL_LOG("%s\n", "ResourceCache destroyed.");
}

ShaderModule &ResourceCache::request_shader_module()
{
//    return ;
}

PipelineLayout &ResourceCache::request_pipeline_layout(const std::vector<ShaderModule> &shaders)
{
    return request_resource(pipeline_layout_mutex, pipeline_layouts, shaders);
}

PipelineLayout &
ResourceCache::request_pipeline_layout(const std::vector<ShaderModule> & shaders,
                                       const VkPipelineLayoutCreateInfo &pipelineLayoutInfo)
{
    return request_resource(pipeline_layout_mutex, pipeline_layouts, shaders, pipelineLayoutInfo);
}

DescriptorPool &ResourceCache::request_descriptor_pool(const VkDescriptorPoolSize &pool_size,
                                                       u32                         maxSets)
{
    return request_resource(descriptor_pool_mutex, descriptor_pools, pool_size, maxSets);
}

DescriptorSetLayout &ResourceCache::request_descriptor_set_layout(
    const std::vector<VkDescriptorSetLayoutBinding> &new_bindings)
{
    return request_resource(descriptor_set_layout_mutex, descriptor_set_layouts, new_bindings);
}

DescriptorSet &ResourceCache::request_descriptor_set(DescriptorSetLayout &layout,
                                                     DescriptorPool &     pool)
{
    return request_resource(descriptor_set_mutex, descriptor_sets, layout, pool);
}

RenderPass &
ResourceCache::request_render_pass(const std::vector<VkAttachmentDescription> &attachments,
                                   const std::vector<SubpassInfo> &            subpasses)
{
    return request_resource(render_pass_mutex, render_passes, attachments, subpasses);
}

RenderPass &ResourceCache::request_render_pass(const std::vector<Attachment> &   attachments,
                                               const std::vector<LoadStoreInfo> &load_store,
                                               const std::vector<LoadStoreInfo> &stencil_load_store,
                                               const std::vector<SubpassInfo> &  subpasses)
{
    //    return request_resource(render_passes, attachments, load_store, stencil_load_store,
    //    subpasses);
}

GraphicsPipeline &ResourceCache::request_graphics_pipeline(PipelineStatus &pipeline_state)
{
    return request_resource(graphics_pipeline_mutex, graphics_pipelines, pipeline_cache,
                            pipeline_state);
}

ComputePipeline &ResourceCache::request_compute_pipeline(PipelineStatus &pipeline_state)
{
    return request_resource(compute_pipeline_mutex, compute_pipelines, pipeline_cache,
                            pipeline_state);
}

FrameBuffer &ResourceCache::request_framebuffer(const std::vector<VkImageView> &imageView,
                                                const VkExtent2D &              extent,
                                                const RenderPass &              renderPass)
{
    return request_resource(framebuffer_mutex, framebuffers, imageView, extent, renderPass);
}

void ResourceCache::summary()
{
    CRUEL_LOG("Cache summary (%p):\n", this);
    {
        std::lock_guard<std::mutex> guard(pipeline_layout_mutex);
        printf("  PipelineLayout in cache (%lu):\n", pipeline_layouts.size());
        for (auto &a:pipeline_layouts)
        {
            printf("\t%20zu : %p\n", a.first, a.second.get_handle());
        }
        printf("  -----------------------------------\n");
    }
    {
        std::lock_guard<std::mutex> guard(descriptor_set_mutex);
        printf("  DesctiprorSets in cache (%lu):\n", descriptor_sets.size());
        for (auto &a:descriptor_sets)
        {
            printf("\t%20zu : %p\n", a.first, a.second.get_handle());
        }
        printf("  -----------------------------------\n");
    }
    {
        std::lock_guard<std::mutex> guard(descriptor_set_layout_mutex);
        printf("  DesctiprorSetLayouts in cache (%lu):\n", descriptor_set_layouts.size());
        for (auto &a:descriptor_set_layouts)
        {
            printf("\t%20zu : %p\n", a.first, a.second.get_handle());
        }
        printf("  -----------------------------------\n");
    }
    {
        std::lock_guard<std::mutex> guard(descriptor_pool_mutex);
        printf("  DesctiprorPools in cache (%lu):\n", descriptor_pools.size());
        for (auto &a:descriptor_pools)
        {
            printf("\t%20zu : %p\n", a.first, a.second.get_handle());
        }
        printf("  -----------------------------------\n");
    }
    {
        std::lock_guard<std::mutex> guard(render_pass_mutex);
        printf("  RenderPasses in cache (%lu):\n", render_passes.size());
        for (auto &a:render_passes)
        {
            printf("\t%20zu : %p\n", a.first, a.second.get_handle());
        }
        printf("  -----------------------------------\n");
    }
    {
        std::lock_guard<std::mutex> guard(graphics_pipeline_mutex);
        printf("  Graphics Pipelines in cache (%lu):\n", graphics_pipelines.size());
        for (auto &a:graphics_pipelines)
        {
            printf("\t%20zu : %p\n", a.first, a.second.get_handle());
        }
        printf("  -----------------------------------\n");
    }
    {
        std::lock_guard<std::mutex> guard(compute_pipeline_mutex);
        printf("  Compute Pipelines in cache (%lu):\n", compute_pipelines.size());
        for (auto &a:compute_pipelines)
        {
            printf("\t%20zu : %p\n", a.first, a.second.get_handle());
        }
        printf("  -----------------------------------\n");
    }
    {
        std::lock_guard<std::mutex> guard(framebuffer_mutex);
        printf("  FrameBuffer in cache (%lu):\n", framebuffers.size());
        for (auto &a:framebuffers)
        {
            printf("\t%20zu : %p\n", a.first, a.second.get_handle());
        }
        printf("  -----------------------------------\n");
    }
}

} // namespace cruelEngine::cruelRender