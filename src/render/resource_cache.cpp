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

} // namespace cruelEngine::cruelRender