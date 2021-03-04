#include "image.hpp"
#include "logical_device.hpp"

namespace cruelEngine {
namespace VulkanContext {
    Image::Image(LogicalDevice &device) : 
        device {device}
    {

    }

    Image::~Image()
    {

    }
    
}
}