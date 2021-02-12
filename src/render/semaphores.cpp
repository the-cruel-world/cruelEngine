#include "semaphores.hpp"


namespace realEngine {
    Semaphores::Semaphores(const VulkanDevice &_device, const SwapChain &_swapChain)
        : device(_device), swapChain(_swapChain)
    {}

    Semaphores::~Semaphores() {}
}