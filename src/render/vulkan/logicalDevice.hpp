#pragma once

#include "vkcommon.h"


namespace cruelEngine {
namespace VulkanContext {
    struct LogicalDevice {
    public:
        LogicalDevice() {}
        ~LogicalDevice() {}

    public:
        VkFramebuffer   &createFrameBuffer();
    };
}
}