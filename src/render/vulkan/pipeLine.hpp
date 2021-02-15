#pragma once
#include "vkcommon.h"

#include "shader.hpp"
#include "vkDevice.hpp"
#include "renderPass.hpp"

namespace cruelEngine {
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