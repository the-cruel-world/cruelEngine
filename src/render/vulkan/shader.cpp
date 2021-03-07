#include "shader.hpp"

#include "logical_device.hpp"

namespace cruelEngine {
namespace cruelRender {
size_t readFile(const std::string &filename, std::vector<char> &source) {
  std::ifstream file(filename, std::ios::ate | std::ios::binary);

  if (!file.is_open()) {
    std::cerr << "readFile: failed to open file: " + filename << std::endl;
    exit(-1);
  }
  size_t fileSize = (size_t)file.tellg();
  source.resize(fileSize);

  file.seekg(0);
  file.read(source.data(), fileSize);
  file.close();
  return fileSize;
}

VkShaderModule createShaderModule(const VkDevice logicalDevice,
                                  const std::vector<char> &code) {
  VkShaderModuleCreateInfo createInfo{};
  createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  createInfo.codeSize = code.size();
  createInfo.pCode = reinterpret_cast<const uint32_t *>(code.data());

  VkShaderModule shaderModule;
  if (vkCreateShaderModule(logicalDevice, &createInfo, nullptr,
                           &shaderModule) != VK_SUCCESS) {
    throw std::runtime_error("failed to create shader module!");
  }
  return shaderModule;
}

void destroyShaderModule(VkDevice &device, VkShaderModule &handle) {
  vkDestroyShaderModule(device, handle, nullptr);
}

ShaderModule::ShaderModule(const LogicalDevice &device,
                           const std::string &filename, const char *entry_point,
                           const VkShaderStageFlagBits stage)
    : device{device}, shader_stage{stage} {
  strncpy(entry, entry_point, 255);
  readFile(filename, source);
}

ShaderModule::~ShaderModule() { source.clear(); }
} // namespace cruelRender
} // namespace cruelEngine