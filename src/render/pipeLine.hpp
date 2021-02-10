#pragma once
#include "../common.h"
#include "../types.h"
#include "../debug/debug.hpp"

#include "shader.hpp"
#include "vkDevice.hpp"
#include "renderPass.hpp"

namespace realEngine {
    class PipeLine {
    public: 
        PipeLine(const VulkanDevice &_device, const SwapChain &_swapChain, const RenderPass &_renderPass);

        virtual ~PipeLine();

    public:
        VkPipeline              pipeLine;
        VkPipelineLayout        pipeLineLayout;

        void                    createPipiLineALayout();

    private:
        const VulkanDevice      &device;
        const SwapChain         &swapChain;
        const RenderPass        &renderPass;
    };
}