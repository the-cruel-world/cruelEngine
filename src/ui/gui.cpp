#include "gui.hpp"

#include "../render/render_session.hpp"

#include "../render/vulkan/buffer.hpp"
#include "../render/vulkan/logical_device.hpp"
#include "../render/vulkan/physical_device.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include "implot.h"

namespace cruelEngine::cruelGui
{
Gui::Gui(cruelRender::RenderSession &session) : Gui{session, GuiUsageFlagBits::GUI_ONLY_IMGUI}
{}

Gui::Gui(cruelRender::RenderSession &session, GuiUsageFlags usage) :
    GuiOverlay{session}, flags{usage}
{
    ImGui::CreateContext();
    if (flags & GuiUsageFlagBits::GUI_ENABLE_IMPLOT)
        ImPlot::CreateContext();

    // Color scheme
    ImGui::StyleColorsLight();
    ImGui::GetStyle().WindowRounding    = 5.0f;
    ImGui::GetStyle().ChildRounding     = 1.0f;
    ImGui::GetStyle().FrameRounding     = 3.0f;
    ImGui::GetStyle().PopupRounding     = 6.0f;
    ImGui::GetStyle().GrabRounding      = 3.0f;
    ImGui::GetStyle().ScrollbarRounding = 2.0f;
    ImGui::GetStyle().Alpha             = 0.90f;

    // Dimensions
    ImGuiIO &io        = ImGui::GetIO();
    io.FontGlobalScale = scale;
#ifdef GUI_DEBUG
    std::cout << "[Gui::Constructor] imgui inited!" << std::endl;
#endif
    prepare_resource();
#ifdef GUI_DEBUG
    std::cout << "[Gui::Constructor] Resource prepared!" << std::endl;
#endif

    prepare_pipeline();
#ifdef GUI_DEBUG
    std::cout << "[Gui::Constructor] Pipeline created!" << std::endl;
#endif

    resize(session.get_swapchain().get_properties().extent.width,
           session.get_swapchain().get_properties().extent.height);
#ifdef GUI_DEBUG
    std::cout << "[Gui::Constructor] display size setted!" << std::endl;
#endif
    ImGui_ImplGlfw_InitForVulkan(&session.get_window().get_handle(), true);

    // ImGui_ImplVulkan_InitInfo init_info = {};
    // init_info.Instance                  = session.getInstance().get_handle();
    // init_info.PhysicalDevice            = session.get_device().get_physicalDevice().get_handle();
    // init_info.Device                    = session.get_device().get_handle();
    // init_info.QueueFamily               = session.get_graphic_queue()->get_family_index();
    // init_info.Queue                     = session.get_graphic_queue()->get_handle();
    // init_info.PipelineCache             = VK_NULL_HANDLE;
    // init_info.DescriptorPool            = descriptorPool->get_handle();
    // init_info.Allocator                 = nullptr;
    // init_info.MinImageCount             = session.get_swapchain().get_properties().image_count;
    // init_info.ImageCount                = session.get_swapchain().get_properties().image_count;
    // ImGui_ImplVulkan_Init(&init_info, session.get_render_pass().get_handle());
}

Gui::~Gui()
{
    free_resource();
}

void Gui::Draw(cruelRender::CommandBuffer &commandBuffer)
{
#ifdef GUI_DEBUG
    std::cout << "[Gui::Draw] recording draw commands." << commandBuffer.get_handle() << std::endl;
#endif
    ImDrawData *imDrawData = ImGui::GetDrawData();
    bool        updated    = false;

    u32 vertexOffset = 0;
    u32 indexOffset  = 0;

    if ((!imDrawData) || (imDrawData->CmdListsCount == 0))
    {
        return;
    }

    ImGuiIO &io = ImGui::GetIO();

    commandBuffer.bindPipeLine(pipeline->get_handle(), VK_PIPELINE_BIND_POINT_GRAPHICS);
    vkCmdBindDescriptorSets(commandBuffer.get_handle(), VK_PIPELINE_BIND_POINT_GRAPHICS,
                            pipelineLayout->get_handle(), 0, 1, &descriptorSet->get_handle(), 0,
                            nullptr);

    pushConstBlock.scale     = glm::vec2(2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y);
    pushConstBlock.translate = glm::vec2(-1.0f);
    vkCmdPushConstants(commandBuffer.get_handle(), pipelineLayout->get_handle(),
                       VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstBlock), &pushConstBlock);

    VkDeviceSize offsets[1] = {0};
    vkCmdBindVertexBuffers(commandBuffer.get_handle(), 0, 1, &vertexBuffer->get_handle(), offsets);
    vkCmdBindIndexBuffer(commandBuffer.get_handle(), indexBuffer->get_handle(), 0,
                         VK_INDEX_TYPE_UINT16);

    for (int32_t i = 0; i < imDrawData->CmdListsCount; i++)
    {
        const ImDrawList *cmd_list = imDrawData->CmdLists[i];
        for (int32_t j = 0; j < cmd_list->CmdBuffer.Size; j++)
        {
            const ImDrawCmd *pcmd = &cmd_list->CmdBuffer[j];
            VkRect2D         scissorRect;
            scissorRect.offset.x      = std::max((int32_t)(pcmd->ClipRect.x), 0);
            scissorRect.offset.y      = std::max((int32_t)(pcmd->ClipRect.y), 0);
            scissorRect.extent.width  = (uint32_t)(pcmd->ClipRect.z - pcmd->ClipRect.x);
            scissorRect.extent.height = (uint32_t)(pcmd->ClipRect.w - pcmd->ClipRect.y);
            vkCmdSetScissor(commandBuffer.get_handle(), 0, 1, &scissorRect);
            vkCmdDrawIndexed(commandBuffer.get_handle(), pcmd->ElemCount, 1, indexOffset,
                             vertexOffset, 0);
            indexOffset += pcmd->ElemCount;
        }
        vertexOffset += cmd_list->VtxBuffer.Size;
    }
#ifdef GUI_DEBUG
    std::cout << "[Gui::Draw] recorded draw commands." << std::endl;
#endif
}

void Gui::update()
{
#ifdef GUI_DEBUG
    std::cout << "[Gui::updateBuffer] updating buffers!" << std::endl;
#endif
    updated = false;
    session.get_graphic_queue()->wait_idle();

    ImDrawData *draw_data = ImGui::GetDrawData();
    if (!draw_data)
    {
        return;
    }

    size_t vertex_buffer_size = draw_data->TotalVtxCount * sizeof(ImDrawVert);
    size_t index_buffer_size  = draw_data->TotalIdxCount * sizeof(ImDrawIdx);

    if ((vertex_buffer_size == 0) || (index_buffer_size == 0))
    {
        return;
    }

    if (vertexBuffer != nullptr)
    {
        auto last_buffer_size = vertexBuffer->get_size();
        if (last_buffer_size < vertex_buffer_size)
        {
            vertexBuffer.reset();
            VkDeviceSize buffer_size =
                std::ceil((float) vertex_buffer_size / (float) guiVertexblockSize) *
                guiVertexblockSize;
            assert(buffer_size >= vertex_buffer_size &&
                   "blocked size must larger than original size");
            vertexBuffer = std::make_shared<cruelRender::Buffer>(
                session.get_device(), buffer_size, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        }
    }
    else
    {
        VkDeviceSize buffer_size =
            std::ceil((float) vertex_buffer_size / (float) guiVertexblockSize) * guiVertexblockSize;
        assert(buffer_size >= vertex_buffer_size && "blocked size must larger than original size");

        vertexBuffer = std::make_shared<cruelRender::Buffer>(session.get_device(), buffer_size,
                                                             VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                                             VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    }
    if (indexBuffer != nullptr)
    {
        auto last_buffer_size = indexBuffer->get_size();
        if (last_buffer_size < index_buffer_size)
        {
            indexBuffer.reset();
            VkDeviceSize buffer_size =
                std::ceil((float) index_buffer_size / (float) guiIndexblockSize) *
                guiIndexblockSize;
            assert(buffer_size >= index_buffer_size &&
                   "blocked size must larger than original size");
            indexBuffer = std::make_shared<cruelRender::Buffer>(
                session.get_device(), buffer_size, VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
        }
    }
    else
    {
        VkDeviceSize buffer_size =
            std::ceil((float) index_buffer_size / (float) guiIndexblockSize) * guiIndexblockSize;
        assert(buffer_size >= index_buffer_size && "blocked size must larger than original size");
        indexBuffer = std::make_shared<cruelRender::Buffer>(session.get_device(), buffer_size,
                                                            VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                                            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
    }

    // copy data
    void *vtx_data, *idx_data;
    vkMapMemory(vertexBuffer->get_device().get_handle(), vertexBuffer->get_handleMem(), 0,
                vertex_buffer_size, 0, &vtx_data);
    vkMapMemory(indexBuffer->get_device().get_handle(), indexBuffer->get_handleMem(), 0,
                index_buffer_size, 0, &idx_data);

    ImDrawVert *vtx_dest = (ImDrawVert *) vtx_data;
    ImDrawIdx * idx_dest = (ImDrawIdx *) idx_data;

    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList *cmd_list = draw_data->CmdLists[n];
        memcpy(vtx_dest, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
        memcpy(idx_dest, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
        vtx_dest += cmd_list->VtxBuffer.Size; // Update the offset
        idx_dest += cmd_list->IdxBuffer.Size; // Update the offset
    }
    vkUnmapMemory(vertexBuffer->get_device().get_handle(), vertexBuffer->get_handleMem());
    vkUnmapMemory(indexBuffer->get_device().get_handle(), indexBuffer->get_handleMem());

#ifdef GUI_DEBUG
    std::cout << "[Gui::updateBuffer] updated buffers!" << std::endl;
#endif
}

void Gui::renderFrame()
{
    ImGuiIO &io    = ImGui::GetIO();
    io.DisplaySize = ImVec2(getSession().get_swapchain().get_properties().extent.width,
                            getSession().get_swapchain().get_properties().extent.height);

    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    if (guiUpdateCb != nullptr)
    {
        guiUpdateCb(this);
    }
    ImGui::Render();
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
    vkDestroyBuffer(session.get_device().get_handle(), stagingBuffer, nullptr);
    vkFreeMemory(session.get_device().get_handle(), stagingBufferMemory, nullptr);

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
    poolSize.type            = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSize.descriptorCount = 1;

    descriptorPool =
        std::make_shared<cruelRender::DescriptorPool>(session.get_device(), poolSize, 2);

    VkDescriptorSetLayoutBinding layoutBinding;
    layoutBinding.descriptorType     = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    layoutBinding.stageFlags         = VK_SHADER_STAGE_FRAGMENT_BIT;
    layoutBinding.binding            = 0;
    layoutBinding.descriptorCount    = 1;
    layoutBinding.pImmutableSamplers = nullptr;

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

    VkWriteDescriptorSet descriptorWriteInfo{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
    descriptorWriteInfo.dstSet          = descriptorSet->get_handle();
    descriptorWriteInfo.dstBinding      = 0;
    descriptorWriteInfo.dstArrayElement = 0;
    descriptorWriteInfo.descriptorType  = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    descriptorWriteInfo.descriptorCount = 1;
    descriptorWriteInfo.pImageInfo      = &imageInfo;

    std::vector<VkWriteDescriptorSet> writeDescriptorSets = {descriptorWriteInfo};

    vkUpdateDescriptorSets(session.get_device().get_handle(),
                           static_cast<u32>(writeDescriptorSets.size()), writeDescriptorSets.data(),
                           0, nullptr);
}

void Gui::resize(uint32_t width, uint32_t height)
{
    ImGuiIO &io    = ImGui::GetIO();
    io.DisplaySize = ImVec2((float) (width), (float) (height));
}

void Gui::prepare_pipeline()
{
    std::vector<cruelRender::ShaderModule> shaderModules{};
    shaderModules.emplace_back(session.get_device(),
                               "../data/shaders/"
                               "frag.spv",
                               "main", VK_SHADER_STAGE_FRAGMENT_BIT);

    shaderModules.emplace_back(session.get_device(),
                               "../data/shaders/"
                               "vert.spv",
                               "main", VK_SHADER_STAGE_VERTEX_BIT);

    VkPushConstantRange pushConstantRange{};
    pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    pushConstantRange.size       = sizeof(PushConstBlock);
    pushConstantRange.offset     = 0;

    VkPipelineLayoutCreateInfo pipelineLayoutCI{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
    pipelineLayoutCI.setLayoutCount         = 1;
    pipelineLayoutCI.pSetLayouts            = &descriptorSetLayout->get_handle();
    pipelineLayoutCI.pushConstantRangeCount = 1;
    pipelineLayoutCI.pPushConstantRanges    = &pushConstantRange;

    pipelineLayout = std::make_shared<cruelRender::PipelineLayout>(session.get_device(),
                                                                   shaderModules, pipelineLayoutCI);
    cruelRender::PipelineStatus                   pipelineState{};
    cruelRender::PipelineStatus::VertexInputState vertex_input_state;

    VkVertexInputAttributeDescription;
    VkVertexInputBindingDescription;

    vertex_input_state.attributes.push_back(
        {0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, pos)});
    vertex_input_state.attributes.push_back(
        {1, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, uv)});
    vertex_input_state.attributes.push_back(
        {2, 0, VK_FORMAT_R8G8B8A8_UNORM, offsetof(ImDrawVert, col)});

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
    if (flags & GuiUsageFlagBits::GUI_ENABLE_IMPLOT)
        ImPlot::DestroyContext();
    ImGui::DestroyContext();

    sampler.reset();
    fontView.reset();
    fontImage.reset();

    destroy_pipeline();

    descriptorSet.reset();
    descriptorSetLayout.reset();
    descriptorPool.reset();

    indexBuffer.reset();
    vertexBuffer.reset();

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
    updated = true;
    va_end(args);
}

} // namespace cruelEngine::cruelGui