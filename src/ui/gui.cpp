#include "gui.hpp"

#include "../render/vulkan/buffer.hpp"
#include "../render/vulkan/logical_device.hpp"
#include "../render/vulkan/physical_device.hpp"

namespace cruelEngine {
Gui::Gui(const cruelRender::LogicalDevice &_device) : device(_device) {
  ImGui::CreateContext();
  // Color scheme
  ImGuiStyle &style = ImGui::GetStyle();
  style.Colors[ImGuiCol_TitleBg] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
  style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.0f, 0.0f, 0.0f, 0.1f);
  style.Colors[ImGuiCol_MenuBarBg] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
  style.Colors[ImGuiCol_Header] = ImVec4(0.8f, 0.0f, 0.0f, 0.4f);
  style.Colors[ImGuiCol_HeaderActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
  style.Colors[ImGuiCol_HeaderHovered] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
  style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.8f);
  style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
  style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
  style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
  style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(1.0f, 1.0f, 1.0f, 0.1f);
  style.Colors[ImGuiCol_FrameBgActive] = ImVec4(1.0f, 1.0f, 1.0f, 0.2f);
  style.Colors[ImGuiCol_Button] = ImVec4(1.0f, 0.0f, 0.0f, 0.4f);
  style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 0.0f, 0.0f, 0.6f);
  style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.0f, 0.0f, 0.0f, 0.8f);
  // Dimensions
  ImGuiIO &io = ImGui::GetIO();
  io.FontGlobalScale = scale;
}

void Gui::prepare_resource() {
  ImGuiIO &io = ImGui::GetIO();

  // Create font texture
  unsigned char *fontData;
  int texWidth, texHeight;

  const std::string filename = "../data/assets/font/Roboto-Medium.ttf";
  io.Fonts->AddFontFromFileTTF(filename.c_str(), 16.0f);

  io.Fonts->GetTexDataAsRGBA32(&fontData, &texWidth, &texHeight);
  VkDeviceSize uploadSize = texWidth * texHeight * 4 * sizeof(char);

  // Create target image for copy
  VkImageCreateInfo imageInfo = {VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
  imageInfo.imageType = VK_IMAGE_TYPE_2D;
  imageInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
  imageInfo.extent.width = texWidth;
  imageInfo.extent.height = texHeight;
  imageInfo.extent.depth = 1;
  imageInfo.mipLevels = 1;
  imageInfo.arrayLayers = 1;
  imageInfo.samples = VK_SAMPLE_COUNT_1_BIT;
  imageInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
  imageInfo.usage =
      VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
  imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
  imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  VK_CHECK_RESULT(
      vkCreateImage(device.get_handle(), &imageInfo, nullptr, &fontImage));
  VkMemoryRequirements memReqs;
  vkGetImageMemoryRequirements(device.get_handle(), fontImage, &memReqs);
  VkMemoryAllocateInfo memAllocInfo = {VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
  memAllocInfo.allocationSize = memReqs.size;
  memAllocInfo.memoryTypeIndex = cruelRender::findMemoryType(
      device.get_physicalDevice().get_handle(), memReqs.memoryTypeBits,
      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
  VK_CHECK_RESULT(vkAllocateMemory(device.get_handle(), &memAllocInfo, nullptr,
                                   &fontMemory));
  VK_CHECK_RESULT(
      vkBindImageMemory(device.get_handle(), fontImage, fontMemory, 0));

  VkImageViewCreateInfo viewInfo = {VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
  viewInfo.image = fontImage;
  viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
  viewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
  viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
  viewInfo.subresourceRange.levelCount = 1;
  viewInfo.subresourceRange.layerCount = 1;
  VK_CHECK_RESULT(
      vkCreateImageView(device.get_handle(), &viewInfo, nullptr, &fontView));

  VkBuffer stagingBuffer;
  VkDeviceMemory stagingBufferMemory;
  //   cruelRender::createBuffer(device.get_handle(),
  //                               device.get_physicalDevice().get_handle(),
  //                               uploadSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
  //                               VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
  //                                   VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
  //                               stagingBuffer, stagingBufferMemory);

  void *data;
  vkMapMemory(device.get_handle(), stagingBufferMemory, 0, uploadSize, 0,
              &data);
  memcpy(data, fontData, (size_t)uploadSize);
  vkUnmapMemory(device.get_handle(), stagingBufferMemory);

  // cruelRender::CommandBuffer;
}

} // namespace cruelEngine