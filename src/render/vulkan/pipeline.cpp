#include "pipeline.hpp"

#include "logical_device.hpp"
#include "render_pass.hpp"
#include "shader.hpp"

namespace cruelEngine {
namespace cruelRender {

PipelineLayout::PipelineLayout(LogicalDevice &_device,
                               const std::vector<ShaderModule> &shaders)
    : device(_device), shaders{shaders} {
  VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
  pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  pipelineLayoutInfo.setLayoutCount = 0;            // Optional
  pipelineLayoutInfo.pSetLayouts = nullptr;         // Optional
  pipelineLayoutInfo.pushConstantRangeCount = 0;    // Optional
  pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

  VK_CHECK_RESULT(vkCreatePipelineLayout(
      device.get_handle(), &pipelineLayoutInfo, nullptr, &handle));
}

PipelineLayout::PipelineLayout(
    LogicalDevice &_device, const std::vector<ShaderModule> &shaders,
    const VkPipelineLayoutCreateInfo &pipelineLayoutInfo)
    : device(_device), shaders{shaders} {
  VK_CHECK_RESULT(vkCreatePipelineLayout(
      device.get_handle(), &pipelineLayoutInfo, nullptr, &handle));
}

PipelineLayout::~PipelineLayout() {
  if (handle != VK_NULL_HANDLE)
    vkDestroyPipelineLayout(device.get_handle(), handle, nullptr);
}

Pipeline::Pipeline(LogicalDevice &device) : device{device} {}

Pipeline::Pipeline(Pipeline &&other)
    : device{other.device}, handle{other.handle}, status{other.status} {
  other.handle = VK_NULL_HANDLE;
}

Pipeline::~Pipeline() {
  if (handle != VK_NULL_HANDLE)
    vkDestroyPipeline(device.get_handle(), handle, nullptr);
}

ComputePipeline::ComputePipeline(LogicalDevice &device,
                                 VkPipelineCache pipeline_cache,
                                 PipelineStatus &pipeline_state)
    : Pipeline{device} {
  const ShaderModule shader_module =
      pipeline_state.get_pipeline_layout()->get_shader_modules().front();

  if (shader_module.get_shader_stage() != VK_SHADER_STAGE_COMPUTE_BIT) {
    throw std::runtime_error(
        "Creating ComputePipeline: Compute shader module required.");
  }

  std::vector<VkShaderModule> shader_modules;
  VkPipelineShaderStageCreateInfo stage{
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
  stage.stage = shader_module.get_shader_stage();
  stage.pName = shader_module.get_entry_point();
  stage.module =
      createShaderModule(device.get_handle(), shader_module.get_source());
  shader_modules.push_back(stage.module);

  // VkSpecializationInfo specialization_info{};

  VkComputePipelineCreateInfo create_info{
      VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO};
  create_info.layout = pipeline_state.get_pipeline_layout()->get_handle();
  create_info.stage = stage;

  VK_CHECK_RESULT(vkCreateComputePipelines(device.get_handle(), pipeline_cache,
                                           1, &create_info, nullptr, &handle));
  for (auto shader_module : shader_modules) {
    vkDestroyShaderModule(device.get_handle(), shader_module, nullptr);
  }
  status = pipeline_state;
}

GraphicsPipeline::GraphicsPipeline(LogicalDevice &device,
                                   VkPipelineCache pipeline_cache,
                                   PipelineStatus &pipeline_state)
    : Pipeline{device} {
  std::vector<VkPipelineShaderStageCreateInfo> stage_create_infos;

  std::vector<VkShaderModule> shader_modules;
  for (auto &shader_module :
       pipeline_state.get_pipeline_layout()->get_shader_modules()) {
    VkPipelineShaderStageCreateInfo stage_create_info{
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};

    stage_create_info.stage = shader_module.get_shader_stage();
    stage_create_info.pName = shader_module.get_entry_point();
    stage_create_info.module =
        createShaderModule(device.get_handle(), shader_module.get_source());
    shader_modules.push_back(stage_create_info.module);
    stage_create_infos.push_back(stage_create_info);
  }

  VkPipelineVertexInputStateCreateInfo vertex_input_state{
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};
  vertex_input_state.pVertexAttributeDescriptions =
      pipeline_state.get_vertex_input_state().attributes.data();
  vertex_input_state.vertexAttributeDescriptionCount =
      (u32)(pipeline_state.get_vertex_input_state().attributes.size());
  vertex_input_state.pVertexBindingDescriptions =
      pipeline_state.get_vertex_input_state().bindings.data();
  vertex_input_state.vertexBindingDescriptionCount =
      (u32)(pipeline_state.get_vertex_input_state().bindings.size());

  VkPipelineInputAssemblyStateCreateInfo input_assembly_state{
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO};
  input_assembly_state.topology =
      pipeline_state.get_input_assembly_state().topology;
  input_assembly_state.primitiveRestartEnable =
      pipeline_state.get_input_assembly_state().primitive_restart_enable;

  VkPipelineViewportStateCreateInfo viewport_state{
      VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO};
  viewport_state.viewportCount =
      pipeline_state.get_viewport_state().viewport_count;
  viewport_state.scissorCount =
      pipeline_state.get_viewport_state().scissor_count;

  VkPipelineRasterizationStateCreateInfo rasterization_state{
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO};

  rasterization_state.depthClampEnable =
      pipeline_state.get_rasterization_state().depth_clamp_enable;
  rasterization_state.rasterizerDiscardEnable =
      pipeline_state.get_rasterization_state().rasterizer_discard_enable;
  rasterization_state.polygonMode =
      pipeline_state.get_rasterization_state().polygon_mode;
  rasterization_state.cullMode =
      pipeline_state.get_rasterization_state().cull_mode;
  rasterization_state.frontFace =
      pipeline_state.get_rasterization_state().front_face;
  rasterization_state.depthBiasEnable =
      pipeline_state.get_rasterization_state().depth_bias_enable;
  rasterization_state.depthBiasClamp =
      pipeline_state.get_rasterization_state().depth_bias_clamp;
  rasterization_state.depthBiasSlopeFactor =
      pipeline_state.get_rasterization_state().depth_bias_slope_factor;
  rasterization_state.lineWidth =
      pipeline_state.get_rasterization_state().line_width;

  VkPipelineMultisampleStateCreateInfo multisample_state{
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO};
  multisample_state.sampleShadingEnable =
      pipeline_state.get_multisample_state().sample_shading_enable;
  multisample_state.rasterizationSamples =
      pipeline_state.get_multisample_state().rasterization_samples;
  multisample_state.minSampleShading =
      pipeline_state.get_multisample_state().min_sample_shading;
  multisample_state.alphaToCoverageEnable =
      pipeline_state.get_multisample_state().alpha_to_coverage_enable;
  multisample_state.alphaToOneEnable =
      pipeline_state.get_multisample_state().alpha_to_one_enable;
  if (pipeline_state.get_multisample_state().sample_mask) {
    multisample_state.pSampleMask =
        &pipeline_state.get_multisample_state().sample_mask;
  }

  VkPipelineDepthStencilStateCreateInfo depth_stencil_state{
      VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
  depth_stencil_state.depthTestEnable =
      pipeline_state.get_depth_stencil_state().depth_test_enable;
  depth_stencil_state.depthWriteEnable =
      pipeline_state.get_depth_stencil_state().depth_write_enable;
  depth_stencil_state.depthCompareOp =
      pipeline_state.get_depth_stencil_state().depth_compare_op;
  depth_stencil_state.depthBoundsTestEnable =
      pipeline_state.get_depth_stencil_state().depth_bounds_test_enable;
  depth_stencil_state.stencilTestEnable =
      pipeline_state.get_depth_stencil_state().stencil_test_enable;
  depth_stencil_state.front.failOp =
      pipeline_state.get_depth_stencil_state().front.fail_op;
  depth_stencil_state.front.passOp =
      pipeline_state.get_depth_stencil_state().front.pass_op;
  depth_stencil_state.front.depthFailOp =
      pipeline_state.get_depth_stencil_state().front.depth_fail_op;
  depth_stencil_state.front.compareOp =
      pipeline_state.get_depth_stencil_state().front.compare_op;
  depth_stencil_state.front.compareMask = ~0U;
  depth_stencil_state.front.writeMask = ~0U;
  depth_stencil_state.front.reference = ~0U;
  depth_stencil_state.back.failOp =
      pipeline_state.get_depth_stencil_state().back.fail_op;
  depth_stencil_state.back.passOp =
      pipeline_state.get_depth_stencil_state().back.pass_op;
  depth_stencil_state.back.depthFailOp =
      pipeline_state.get_depth_stencil_state().back.depth_fail_op;
  depth_stencil_state.back.compareOp =
      pipeline_state.get_depth_stencil_state().back.compare_op;
  depth_stencil_state.back.compareMask = ~0U;
  depth_stencil_state.back.writeMask = ~0U;
  depth_stencil_state.back.reference = ~0U;

  VkPipelineColorBlendStateCreateInfo color_blend_state{
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};
  color_blend_state.logicOpEnable =
      pipeline_state.get_color_blend_state().logic_op_enable;
  color_blend_state.logicOp = pipeline_state.get_color_blend_state().logic_op;
  color_blend_state.attachmentCount =
      (u32)(pipeline_state.get_color_blend_state().attachments.size());
  color_blend_state.pAttachments =
      reinterpret_cast<const VkPipelineColorBlendAttachmentState *>(
          pipeline_state.get_color_blend_state().attachments.data());
  color_blend_state.blendConstants[0] =
      pipeline_state.get_color_blend_state().blend_constants[0];
  color_blend_state.blendConstants[1] =
      pipeline_state.get_color_blend_state().blend_constants[1];
  color_blend_state.blendConstants[2] =
      pipeline_state.get_color_blend_state().blend_constants[2];
  color_blend_state.blendConstants[3] =
      pipeline_state.get_color_blend_state().blend_constants[3];

  VkPipelineDynamicStateCreateInfo dynamic_state{
      VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO};
  dynamic_state.pDynamicStates =
      pipeline_state.get_dynamic_state().dynamic_states.data();
  dynamic_state.dynamicStateCount =
      (u32)(pipeline_state.get_dynamic_state().dynamic_states.size());

  VkGraphicsPipelineCreateInfo create_info{
      VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
  create_info.stageCount = (u32)stage_create_infos.size();
  create_info.pStages = stage_create_infos.data();

  create_info.pVertexInputState = &vertex_input_state;
  create_info.pInputAssemblyState = &input_assembly_state;
  create_info.pViewportState = &viewport_state;
  create_info.pRasterizationState = &rasterization_state;
  create_info.pMultisampleState = &multisample_state;
  create_info.pDepthStencilState = &depth_stencil_state;
  create_info.pColorBlendState = &color_blend_state;
  create_info.pDynamicState = &dynamic_state;

  create_info.layout = pipeline_state.get_pipeline_layout()->get_handle();
  create_info.renderPass = pipeline_state.get_render_pass()->get_handle();
  create_info.subpass = pipeline_state.get_subpass_index();

  VK_CHECK_RESULT(vkCreateGraphicsPipelines(device.get_handle(), pipeline_cache,
                                            1, &create_info, nullptr, &handle));

  for (auto shader_module : shader_modules) {
    vkDestroyShaderModule(device.get_handle(), shader_module, nullptr);
  }

  status = pipeline_state;
}
} // namespace cruelRender
} // namespace cruelEngine