#include "image.hpp"

#include "logical_device.hpp"

namespace cruelEngine
{
namespace cruelRender
{
Image::Image(LogicalDevice &device) : device{device}
{}

Image::~Image()
{}

} // namespace cruelRender
} // namespace cruelEngine