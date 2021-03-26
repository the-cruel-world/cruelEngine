#pragma once

#include "../vkcommon.h"

namespace cruelEngine::cruelRender
{
class LogicalDevice;

class Sampler
{
public:
    Sampler(LogicalDevice &device, const VkSamplerCreateInfo &info);

    Sampler(const Sampler &) = delete;

    Sampler(Sampler &&sampler);

    Sampler &operator=(const Sampler &) = delete;

    Sampler &operator=(Sampler &&) = delete;

    ~Sampler();

    VkSampler get_handle() const;

private:
    LogicalDevice &device;

    VkSampler handle = VK_NULL_HANDLE;
};
} // namespace cruelEngine::cruelRender