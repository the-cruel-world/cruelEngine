#include "../common.h"
#include "../types.h"

#include "vkDevice.hpp"
#include "swapChain.hpp"

#include "../debug/debug.hpp"

namespace realEngine {
    class Semaphores {
    public:
        Semaphores(const VulkanDevice &_device, const SwapChain &_swapChain);
        virtual ~Semaphores();

    public:

    private:
        const VulkanDevice          &device;
        const SwapChain             &swapChain;
    };
}