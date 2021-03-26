#pragma once

#include "../vkcommon.h"

namespace cruelEngine
{
namespace cruelRender
{
class LogicalDevice;

size_t readFile(const std::string &filename, std::vector<char> &source);

VkShaderModule createShaderModule(const VkDevice logicalDevice, const std::vector<char> &code);

enum class ShaderResourceType
{
    Input = 0,
    InputAttachment,
    Output,
    Image,
    ImageSampler,
    Sampler,
    BufferUniform,
    PushConstant,
    All
};

struct ShaderResource
{
    VkShaderStageFlags stages;
    ShaderResourceType type;

    u32 set;
    u32 binding;
    u32 location;
    u32 vecSize;
    u32 arraySize;
    u32 offSet;
    u32 size;

    std::string name;
};

class ShaderModule
{
public:
    ShaderModule(const LogicalDevice &device, const std::string &filename, const char *entry_point,
                 const VkShaderStageFlagBits stage);

    virtual ~ShaderModule();

    VkShaderStageFlagBits get_shader_stage() const
    {
        return shader_stage;
    }
    const char *get_entry_point() const
    {
        return entry;
    }

    const std::vector<char> &get_source() const
    {
        return source;
    }

    const LogicalDevice &get_device() const
    {
        return device;
    }

private:
    const LogicalDevice & device;
    char                  entry[255];
    std::vector<char>     source;
    VkShaderStageFlagBits shader_stage;
};
} // namespace cruelRender
} // namespace cruelEngine