#pragma once
#include "../vkcommon.h"
#include "shader.hpp"

namespace cruelEngine {
namespace cruelRender {

class ShaderModule;
class RenderPass;
class LogicalDevice;

class PipelineLayout {
public:
  PipelineLayout(LogicalDevice &_device,
                 const std::vector<ShaderModule> &shaders);

  PipelineLayout(LogicalDevice &_device,
                 const std::vector<ShaderModule> &shaders,
                 const VkPipelineLayoutCreateInfo &pipelineLayoutInfo);

  ~PipelineLayout();

  const VkPipelineLayout &get_handle() const { return handle; }

  const LogicalDevice &get_device() const { return device; }

  const std::vector<ShaderModule> &get_shader_modules() const {
    return shaders;
  }

private:
  LogicalDevice &device;

  VkPipelineLayout handle = VK_NULL_HANDLE;

  const std::vector<ShaderModule> &shaders;

  //! descriptor pool
  //! descriptor sets
};

//! Special constant not supported here
class PipelineStatus {
public:
  void reset() {
    pipeline_layout = nullptr;
    render_pass = nullptr;
    vertex_input_sate = {};
    rasterization_state = {};
    viewport_state = {};
    multisample_state = {};
    depth_stencil_state = {};
    color_blend_state = {};
    subpass_index = 0;
  };
  struct VertexInputState {
    std::vector<VkVertexInputBindingDescription> bindings;
    std::vector<VkVertexInputAttributeDescription> attributes;
  };

  struct ViewportState {
    uint32_t viewport_count{1};
    uint32_t scissor_count{1};
  };

  struct InputAssemblyState {
    VkPrimitiveTopology topology{VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST};
    VkBool32 primitive_restart_enable{VK_FALSE};
  };

  struct MultisampleState {
    VkSampleCountFlagBits rasterization_samples{VK_SAMPLE_COUNT_1_BIT};
    VkBool32 sample_shading_enable{VK_FALSE};

    float min_sample_shading{0.0f};
    VkSampleMask sample_mask{0};

    VkBool32 alpha_to_coverage_enable{VK_FALSE};
    VkBool32 alpha_to_one_enable{VK_FALSE};
  };

  struct RasterizationState {
    VkBool32 depth_clamp_enable{VK_FALSE};
    VkBool32 rasterizer_discard_enable{VK_FALSE};

    VkPolygonMode polygon_mode{VK_POLYGON_MODE_FILL};
    VkCullModeFlags cull_mode{VK_CULL_MODE_BACK_BIT};

    VkFrontFace front_face{VK_FRONT_FACE_COUNTER_CLOCKWISE};
    VkBool32 depth_bias_enable{VK_FALSE};

    float depth_bias_clamp{1.0f};
    float depth_bias_slope_factor{1.0f};
    float line_width{1.0f};
  };

  struct StencilOpState {
    VkStencilOp fail_op{VK_STENCIL_OP_REPLACE};
    VkStencilOp pass_op{VK_STENCIL_OP_REPLACE};

    VkStencilOp depth_fail_op{VK_STENCIL_OP_REPLACE};
    VkCompareOp compare_op{VK_COMPARE_OP_NEVER};
  };

  struct ColorBlendAttachmentState {
    VkBool32 blend_enable{VK_FALSE};

    VkBlendFactor src_color_blend_factor{VK_BLEND_FACTOR_ONE};
    VkBlendFactor dst_color_blend_factor{VK_BLEND_FACTOR_ZERO};
    VkBlendOp color_blend_op{VK_BLEND_OP_ADD};

    VkBlendFactor src_alpha_blend_factor{VK_BLEND_FACTOR_ONE};
    VkBlendFactor dst_alpha_blend_factor{VK_BLEND_FACTOR_ZERO};
    VkBlendOp alpha_blend_op{VK_BLEND_OP_ADD};

    VkColorComponentFlags color_write_mask{
        VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT};
  };

  struct ColorBlendState {
    VkBool32 logic_op_enable{VK_FALSE};
    VkLogicOp logic_op{VK_LOGIC_OP_CLEAR};
    std::vector<ColorBlendAttachmentState> attachments;
    float blend_constants[4]{1.0f, 1.0f, 1.0f, 1.0f};
  };

  struct DepthStencilState {
    VkBool32 depth_test_enable{VK_TRUE};
    VkBool32 depth_write_enable{VK_TRUE};

    // Note: Using Reversed depth-buffer for increased precision, so Greater
    // depth values are kept
    VkCompareOp depth_compare_op{VK_COMPARE_OP_GREATER};
    VkBool32 depth_bounds_test_enable{VK_FALSE};

    VkBool32 stencil_test_enable{VK_FALSE};

    StencilOpState front{};
    StencilOpState back{};
  };

  struct DynamicState {
    std::vector<VkDynamicState> dynamic_states{
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_LINE_WIDTH,
        VK_DYNAMIC_STATE_DEPTH_BIAS,
        VK_DYNAMIC_STATE_BLEND_CONSTANTS,
        VK_DYNAMIC_STATE_DEPTH_BOUNDS,
        VK_DYNAMIC_STATE_STENCIL_COMPARE_MASK,
        VK_DYNAMIC_STATE_STENCIL_WRITE_MASK,
        VK_DYNAMIC_STATE_STENCIL_REFERENCE,
    };
  };

  void set_render_pass(const RenderPass &new_render_pass) {
    render_pass = &new_render_pass;
  }
  void set_pipeline_layout(PipelineLayout &new_pipeline_layout) {
    pipeline_layout = &new_pipeline_layout;
  }

  void set_vertex_input_state(const VertexInputState &new_vertex_input_sate) {
    vertex_input_sate = new_vertex_input_sate;
  }

  void
  set_input_assembly_state(const InputAssemblyState &new_input_assembly_state) {
    input_assembly_state = new_input_assembly_state;
  }

  void
  set_rasterization_state(const RasterizationState &new_rasterization_state) {
    rasterization_state = new_rasterization_state;
  }

  void set_viewport_state(const ViewportState &new_viewport_state) {
    viewport_state = new_viewport_state;
  }

  void set_multisample_state(const MultisampleState &new_multisample_state) {
    multisample_state = new_multisample_state;
  }

  void
  set_depth_stencil_state(const DepthStencilState &new_depth_stencil_state) {
    depth_stencil_state = new_depth_stencil_state;
  }

  void set_color_blend_state(const ColorBlendState &new_color_blend_state) {
    color_blend_state = new_color_blend_state;
  }

  void set_dynamic_state(const DynamicState &new_dynamic_state) {
    dynamic_state = new_dynamic_state;
  }

  void set_subpass_index(uint32_t new_subpass_index) {
    subpass_index = new_subpass_index;
  }

  PipelineLayout *get_pipeline_layout() const { return pipeline_layout; }
  const RenderPass *get_render_pass() const { return render_pass; }
  const VertexInputState &get_vertex_input_state() const {
    return vertex_input_sate;
  }
  const InputAssemblyState &get_input_assembly_state() const {
    return input_assembly_state;
  }
  const RasterizationState &get_rasterization_state() const {
    return rasterization_state;
  }
  const ViewportState &get_viewport_state() const { return viewport_state; }
  const MultisampleState &get_multisample_state() const {
    return multisample_state;
  }
  const DepthStencilState &get_depth_stencil_state() const {
    return depth_stencil_state;
  }
  const ColorBlendState &get_color_blend_state() const {
    return color_blend_state;
  }
  const DynamicState &get_dynamic_state() const { return dynamic_state; }
  const u32 get_subpass_index() const { return subpass_index; }

protected:
  PipelineLayout *pipeline_layout{nullptr};

  const RenderPass *render_pass{nullptr};

  VertexInputState vertex_input_sate{};

  InputAssemblyState input_assembly_state{};

  RasterizationState rasterization_state{};

  ViewportState viewport_state{};

  MultisampleState multisample_state{};

  DepthStencilState depth_stencil_state{};

  ColorBlendState color_blend_state{};

  DynamicState dynamic_state{};

  u32 subpass_index{0U};
};

/**
 * ! \brief Pipeline base class.
 */
class Pipeline {
public:
  Pipeline(LogicalDevice &device);

  Pipeline(const Pipeline &) = delete;

  Pipeline &operator=(const Pipeline &) = delete;

  Pipeline &operator=(Pipeline &&) = delete;

  Pipeline(Pipeline &&other);

  virtual ~Pipeline();

public:
  void update(const VkGraphicsPipelineCreateInfo &pipelineInfo);

  VkPipeline &get_handle() { return handle; }

  const PipelineStatus &get_status() const { return status; }

protected:
  LogicalDevice &device;

  VkPipeline handle = VK_NULL_HANDLE;

  PipelineStatus status;
};

/**
 * ! \brief compute pipeline
 */
class ComputePipeline : public Pipeline {
public:
  ComputePipeline(LogicalDevice &device, VkPipelineCache pipeline_cache,
                  PipelineStatus &pipeline_state);
  virtual ~ComputePipeline() = default;
};

/**
 * ! \brief graphic pipeline
 */
class GraphicsPipeline : public Pipeline {
public:
  GraphicsPipeline(LogicalDevice &device, VkPipelineCache pipeline_cache,
                   PipelineStatus &pipeline_state);
  virtual ~GraphicsPipeline() = default;
};
} // namespace cruelRender
} // namespace cruelEngine