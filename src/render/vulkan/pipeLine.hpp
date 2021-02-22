#pragma once
#include "vkcommon.h"

#include "shader.hpp"
#include "vkDevice.hpp"
#include "renderPass.hpp"

namespace cruelEngine {
namespace VulkanContext {

    struct PipeLineLayout {
        PipeLineLayout(const VulkanDevice &_device);

        ~PipeLineLayout();

        void                    createPipeLineLayout();

        const VkPipelineLayout  &get_handle() const {return handle;}

        const VulkanDevice      &get_device() const {return device;}

    private:
        const VulkanDevice      &device;

        VkPipelineLayout        handle = VK_NULL_HANDLE;
    };

    struct PipeLine {
    public:
        PipeLine(const VulkanDevice &_device);

        virtual ~PipeLine();

    public:

        void                    createPipeLine(const VkGraphicsPipelineCreateInfo &pipelineInfo);

        void                    update(const VkGraphicsPipelineCreateInfo &pipelineInfo);

        const VkPipeline        &get_handle() const {return handle;}

        const VulkanDevice      &get_device() const {return device;}

        void                    __destroy();

    private:

        const VulkanDevice      &device;

        VkPipeline              handle = VK_NULL_HANDLE;
    };
}
}