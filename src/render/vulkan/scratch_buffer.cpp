#include "scratch_buffer.hpp"

#include "logical_device.hpp"

namespace cruelEngine::cruelRender
{
ScratchBuffer::ScratchBuffer(LogicalDevice &device) : device{device}
{}

ScratchBuffer::~ScratchBuffer()
{}
} // namespace cruelEngine::cruelRender