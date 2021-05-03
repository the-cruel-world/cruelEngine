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

DescriptorSet &ResourceCache::request_descriptor_set(DescriptorSetLayout &layout,
                                                     DescriptorPool &     pool)
{
    return request_resource(descriptor_set_mutex, descriptor_sets, layout, pool);
}
DescriptorSetLayout &ResourceCache::request_descriptor_set_layout(
    const std::vector<VkDescriptorSetLayoutBinding> &new_bindings)
{
    return request_resource(descriptor_set_layout_mutex, descriptor_set_layouts, new_bindings);
}
DescriptorPool &ResourceCache::request_descriptor_pool(const VkDescriptorPoolSize &pool_size,
                                                       u32                         maxSets)
{
    return request_resource(descriptor_pool_mutex, descriptor_pools, pool_size, maxSets);
}
FrameBuffer &ResourceCache::request_framebuffer(const std::vector<VkImageView> &imageView,
                                                const VkExtent2D &              extent,
                                                const RenderPass &              renderPass)
{
    return request_resource(framebuffer_mutex, framebuffers, imageView, extent, renderPass);
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

} // namespace cruelEngine::cruelRender