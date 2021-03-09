#pragma once
#include "../vkcommon.h"
#include "buffer.hpp"
#include "descriptor.hpp"
#include "frame_buffer.hpp"
#include "pipeline.hpp"
#include "render_pass.hpp"
#include "shader.hpp"

namespace cruelEngine {
namespace cruelRender {

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

/**
 * \brief Resource cache. It manages the resources that creates on gpu includes
 * framebuffer, buffer, pipeline, renderpass, shadermodule, etc.
 */
class ResourceCache {
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

  DescriptorSetLayout &request_descriptor_set_layout();

  GraphicsPipeline &request_graphics_pipeline();

  ComputePipeline &request_compute_pipeline();

  DescriptorSet &request_descriptor_set();

  RenderPass &request_render_pass();

  FrameBuffer &request_framebuffer();

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

  std::unordered_map<std::size_t, ShaderModule> shader_modules;

  std::unordered_map<std::size_t, PipelineLayout> pipeline_layouts;

  std::unordered_map<std::size_t, DescriptorSetLayout> descriptor_set_layouts;

  std::unordered_map<std::size_t, DescriptorPool> descriptor_pools;

  std::unordered_map<std::size_t, RenderPass> render_passes;

  std::unordered_map<std::size_t, GraphicsPipeline> graphics_pipelines;

  std::unordered_map<std::size_t, ComputePipeline> compute_pipelines;

  std::unordered_map<std::size_t, DescriptorSet> descriptor_sets;

  std::unordered_map<std::size_t, FrameBuffer> framebuffers;

  // mutex
  std::mutex descriptor_set_mutex;

  std::mutex pipeline_layout_mutex;

  std::mutex shader_module_mutex;

  std::mutex descriptor_set_layout_mutex;

  std::mutex graphics_pipeline_mutex;

  std::mutex render_pass_mutex;

  std::mutex compute_pipeline_mutex;

  std::mutex framebuffer_mutex;
};
} // namespace cruelRender
} // namespace cruelEngine