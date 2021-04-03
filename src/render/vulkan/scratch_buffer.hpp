#pragma once

#include "../vkcommon.h"

namespace cruelEngine::cruelRender
{
class LogicalDevice;

class ScratchBuffer
{
public:
    ScratchBuffer(LogicalDevice &device);

    ~ScratchBuffer();

private:
    LogicalDevice &device;
};
} // namespace cruelEngine::cruelRender