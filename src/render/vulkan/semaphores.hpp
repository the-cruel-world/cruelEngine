#pragma once

#include "vkcommon.h"

#include "vkDevice.hpp"
#include "swapChain.hpp"


namespace cruelEngine {
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