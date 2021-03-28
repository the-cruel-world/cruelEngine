#include "gui.hpp"

#include "../render/render_session.hpp"

#include "../render/vulkan/buffer.hpp"
#include "../render/vulkan/logical_device.hpp"
#include "../render/vulkan/physical_device.hpp"

namespace cruelEngine::cruelGui
{
Gui::Gui(cruelRender::RenderSession &session) : GuiOverlay{session}
{
    ImGui::CreateContext();
    // Color scheme
    ImGuiStyle &style                       = ImGui::GetStyle();
    style.Colors[ImGuiCol_TitleBg]          = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive]    = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 0.0f, 0.0f, 0.1f);
    style.Colors[ImGuiCol_MenuBarBg]        = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
    style.Colors[ImGuiCol_Header]           = ImVec4(0.8f, 0.0f, 0.0f, 0.4f);
    style.Colors[ImGuiCol_HeaderActive]     = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
    style.Colors[ImGuiCol_HeaderHovered]    = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
    style.Colors[ImGuiCol_FrameBg]          = ImVec4(0.0f, 0.0f, 0.0f, 0.8f);
    style.Colors[ImGuiCol_CheckMark]        = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
    style.Colors[ImGuiCol_SliderGrab]       = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
    style.Colors[ImGuiCol_FrameBgHovered]   = ImVec4(1.0f, 1.0f, 1.0f, 0.1f);
    style.Colors[ImGuiCol_FrameBgActive]    = ImVec4(1.0f, 1.0f, 1.0f, 0.2f);
    style.Colors[ImGuiCol_Button]           = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
    style.Colors[ImGuiCol_ButtonHovered]    = ImVec4(1.0f, 0.0f, 0.0f, 0.6f);
    style.Colors[ImGuiCol_ButtonActive]     = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
    // Dimensions
    ImGuiIO &io        = ImGui::GetIO();
    io.FontGlobalScale = scale;

    std::cout << "imgui inited!" << std::endl;

    prepare_resource();
    std::cout << "Resource prepared!" << std::endl;

    prepare_pipeline();
    std::cout << "Pipeline created!" << std::endl;
}

Gui::~Gui()
{
    free_resource();
}

void Gui::Draw(cruelRender::CommandBuffer &commandBuffer)
{}

void Gui::newFrame()
{
    ImGui::NewFrame();
}

void Gui::prepare_resource()
{
    ImGuiIO &io = ImGui::GetIO();

    // Create font texture
    unsigned char *fontData;
    int            texWidth, texHeight;

    const std::string filename = "../data/assets/font/Roboto-Medium.ttf";
    io.Fonts->AddFontFromFileTTF(filename.c_str(), 16.0f);

    io.Fonts->GetTexDataAsRGBA32(&fontData, &texWidth, &texHeight);
    VkDeviceSize uploadSize = texWidth * texHeight * 4 * sizeof(char);

    std::vector<u32> queueFamilies = {session.get_graphic_queue()->get_family_index()};

    VkExtent3D extent{static_cast<uint32_t>(texWidth), static_cast<uint32_t>(texHeight), 1};
    fontImage = std::make_shared<cruelRender::Image>(
        session.get_device(), extent, VK_FORMAT_R8G8B8A8_UNORM,
        VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, VK_SAMPLE_COUNT_1_BIT, 1, 1, VK_IMAGE_TILING_OPTIMAL,
        0, queueFamilies.size(), queueFamilies.data());

    fontView = std::make_shared<cruelRender::ImageView>(session.get_device(), fontImage.get(),
                                                        VK_IMAGE_VIEW_TYPE_2D,
                                                        VK_FORMAT_R8G8B8A8_UNORM, 0, 0, 1, 1);

    VkBuffer       stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    cruelRender::create_buffer(
        session.get_device().get_handle(), session.get_device().get_physicalDevice().get_handle(),
        uploadSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
        stagingBufferMemory);

    void *data;
    vkMapMemory(session.get_device().get_handle(), stagingBufferMemory, 0, uploadSize, 0, &data);
    memcpy(data, fontData, (size_t) uploadSize);
    vkUnmapMemory(session.get_device().get_handle(), stagingBufferMemory);

    /** Todo
     * Create texture image
     * 1. set image layout
     * 2. copy data to image
     * 3. set image layout again
     */
    auto &singleTimeCmd =
        session.get_device().request_commandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY);

    singleTimeCmd.beginOneTime();

    //! Set image layout
    cruelRender::SetImageLayout(fontImage->get_handle(), fontImage->get_format(),
                                VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                singleTimeCmd);

    //! copy buffer to image
    VkBufferImageCopy bufferCopyRegion           = {};
    bufferCopyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    bufferCopyRegion.imageSubresource.layerCount = 1;
    bufferCopyRegion.imageExtent.width           = texWidth;
    bufferCopyRegion.imageExtent.height          = texHeight;
    bufferCopyRegion.imageExtent.depth           = 1;
    vkCmdCopyBufferToImage(singleTimeCmd.get_handle(), stagingBuffer, fontImage->get_handle(),
                           VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bufferCopyRegion);

    cruelRender::SetImageLayout(fontImage->get_handle(), fontImage->get_format(),
                                VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
                                VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, singleTimeCmd);
    singleTimeCmd.endOneTime();

    // Create a Sampler
    VkSamplerCreateInfo samplerCI{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
    samplerCI.magFilter    = VK_FILTER_LINEAR;
    samplerCI.minFilter    = VK_FILTER_LINEAR;
    samplerCI.mipLodBias   = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerCI.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCI.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCI.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerCI.borderColor  = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    sampler = std::make_shared<cruelRender::Sampler>(session.get_device(), samplerCI);

    // Create descriptor (sets, pool, setlayouts)
    VkDescriptorPoolSize poolSize;
    poolSize.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = 1;

    descriptorPool = std::make_shared<cruelRender::DescriptorPool>(session.get_device(), poolSize);

    VkDescriptorSetLayoutBinding layoutBinding;
    layoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    layoutBinding.stageFlags     = VK_SHADER_STAGE_FRAGMENT_BIT;
    layoutBinding.binding        = 0;
    std::vector<VkDescriptorSetLayoutBinding> layoutBindings = {
        layoutBinding,
    };
    descriptorSetLayout =
        std::make_shared<cruelRender::DescriptorSetLayout>(session.get_device(), layoutBindings);

    descriptorSet = std::make_shared<cruelRender::DescriptorSet>(
        session.get_device(), *descriptorSetLayout, *descriptorPool);

    // Update descriptor set info
    VkDescriptorImageInfo imageInfo{};
    imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    imageInfo.imageView   = fontView->get_handle();
    imageInfo.sampler     = sampler->get_handle();

    VkWriteDescriptorSet              descriptorWriteInfo{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
    descriptorWriteInfo.dstSet          = descriptorSet->get_handle();
    descriptorWriteInfo.dstBinding      = 1;
    descriptorWriteInfo.dstArrayElement = 0;
    descriptorWriteInfo.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWriteInfo.descriptorCount = 1;
    descriptorWriteInfo.pImageInfo      = &imageInfo;

    std::vector<VkWriteDescriptorSet> writeDescriptorSets = {descriptorWriteInfo};

    vkUpdateDescriptorSets(session.get_device().get_handle(),
                           static_cast<u32>(writeDescriptorSets.size()), writeDescriptorSets.data(),
                           0, nullptr);
}

void Gui::prepare_pipeline()
{
    std::vector<cruelRender::ShaderModule> shaderModules{};

    VkPipelineLayoutCreateInfo pipelineLayoutCI{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
    pipelineLayoutCI.setLayoutCount         = 1;
    pipelineLayoutCI.pSetLayouts            = &descriptorSetLayout->get_handle();
    pipelineLayoutCI.pushConstantRangeCount = 0;
    pipelineLayoutCI.pPushConstantRanges    = nullptr;

    pipelineLayout = std::make_shared<cruelRender::PipelineLayout>(session.get_device(),
                                                                   shaderModules, pipelineLayoutCI);
    cruelRender::PipelineStatus                   pipelineState{};
    cruelRender::PipelineStatus::VertexInputState vertex_input_state;

    vertex_input_state.attributes.push_back(
        {0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, pos)});
    vertex_input_state.attributes.push_back(
        {0, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, uv)});
    vertex_input_state.attributes.push_back(
        {0, 2, VK_FORMAT_R8G8B8A8_UNORM, offsetof(ImDrawVert, col)});

    vertex_input_state.bindings.push_back({0, sizeof(ImDrawVert), VK_VERTEX_INPUT_RATE_VERTEX});

    cruelRender::PipelineStatus::InputAssemblyState input_assembly_state{};
    input_assembly_state.topology                 = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    input_assembly_state.primitive_restart_enable = VK_FALSE;

    cruelRender::PipelineStatus::RasterizationState rasterization_state = {
        VK_FALSE,
        VK_FALSE,
        VK_POLYGON_MODE_FILL,
        VK_CULL_MODE_NONE,
        VK_FRONT_FACE_COUNTER_CLOCKWISE,
        VK_FALSE};
    cruelRender::PipelineStatus::ViewportState    viewport_state;
    cruelRender::PipelineStatus::MultisampleState multisample_state = {
        VK_SAMPLE_COUNT_1_BIT, VK_FALSE, 1.0f, VK_NULL_HANDLE, VK_FALSE, VK_FALSE};

    cruelRender::PipelineStatus::DepthStencilState depth_stencil_state{};
    depth_stencil_state.depth_compare_op = VK_COMPARE_OP_ALWAYS;

    cruelRender::PipelineStatus::ColorBlendAttachmentState color_attachment{};
    color_attachment.blend_enable     = VK_TRUE;
    color_attachment.color_write_mask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                                        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    color_attachment.src_color_blend_factor = VK_BLEND_FACTOR_SRC_ALPHA;
    color_attachment.dst_color_blend_factor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    color_attachment.color_blend_op         = VK_BLEND_OP_ADD;
    color_attachment.src_alpha_blend_factor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    color_attachment.dst_alpha_blend_factor = VK_BLEND_FACTOR_ZERO;
    color_attachment.alpha_blend_op         = VK_BLEND_OP_ADD;

    cruelRender::PipelineStatus::ColorBlendState color_blend_state{};
    color_blend_state.attachments.push_back(color_attachment);

    cruelRender::PipelineStatus::DynamicState dynamic_state = {
        {VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR}};

    pipelineState.set_vertex_input_state(vertex_input_state);
    pipelineState.set_input_assembly_state(input_assembly_state);
    pipelineState.set_rasterization_state(rasterization_state);
    pipelineState.set_viewport_state(viewport_state);
    pipelineState.set_multisample_state(multisample_state);
    pipelineState.set_depth_stencil_state(depth_stencil_state);
    pipelineState.set_color_blend_state(color_blend_state);
    pipelineState.set_dynamic_state(dynamic_state);
    pipelineState.set_pipeline_layout(*pipelineLayout);
    pipelineState.set_render_pass(session.get_render_pass());

    pipeline = std::make_shared<cruelRender::GraphicsPipeline>(
        session.get_device(), (VkPipelineCache) VK_NULL_HANDLE, pipelineState);
}

void Gui::free_resource()
{
    sampler.reset();
    fontView.reset();
    fontImage.reset();

    destroy_pipeline();

    descriptorSet.reset();
    descriptorSetLayout.reset();
    descriptorPool.reset();

    queue.reset();
}

void Gui::destroy_pipeline()
{
    pipeline.reset();
    pipelineLayout.reset();
}

bool Gui::header(const char *caption)
{
    return ImGui::CollapsingHeader(caption, ImGuiTreeNodeFlags_DefaultOpen);
}

bool Gui::checkBox(const char *caption, bool *value)
{
    bool res = ImGui::Checkbox(caption, value);
    if (res)
    {
        updated = true;
    };
    return res;
}

bool Gui::checkBox(const char *caption, int32_t *value)
{
    bool val = (*value == 1);
    bool res = ImGui::Checkbox(caption, &val);
    *value   = val;
    if (res)
    {
        updated = true;
    };
    return res;
}

bool Gui::inputFloat(const char *caption, float *value, float step, uint32_t precision)
{
    char precision_str[10] = {};
    snprintf(precision_str, 10, "%%.%df", precision);
    bool res = ImGui::InputFloat(caption, value, step, step * 10.0f, precision_str);
    if (res)
    {
        updated = true;
    };
    return res;
}

bool Gui::sliderFloat(const char *caption, float *value, float min, float max)
{
    bool res = ImGui::SliderFloat(caption, value, min, max);
    if (res)
    {
        updated = true;
    };
    return res;
}

bool Gui::sliderInt(const char *caption, int32_t *value, int32_t min, int32_t max)
{
    bool res = ImGui::SliderInt(caption, value, min, max);
    if (res)
    {
        updated = true;
    };
    return res;
}

bool Gui::comboBox(const char *caption, int32_t *itemindex, std::vector<std::string> items)
{
    if (items.empty())
    {
        return false;
    }
    std::vector<const char *> charitems;
    charitems.reserve(items.size());
    for (size_t i = 0; i < items.size(); i++)
    {
        charitems.push_back(items[i].c_str());
    }
    uint32_t itemCount = static_cast<uint32_t>(charitems.size());
    bool     res       = ImGui::Combo(caption, itemindex, &charitems[0], itemCount, itemCount);
    if (res)
    {
        updated = true;
    };
    return res;
}

bool Gui::button(const char *caption)
{
    bool res = ImGui::Button(caption);
    if (res)
    {
        updated = true;
    };
    return res;
}

void Gui::text(const char *formatstr, ...)
{
    va_list args;
    va_start(args, formatstr);
    ImGui::TextV(formatstr, args);
    va_end(args);
}

} // namespace cruelEngine::cruelGui