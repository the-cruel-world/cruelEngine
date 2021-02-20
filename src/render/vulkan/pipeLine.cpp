#include "pipeLine.hpp"

namespace cruelEngine {
namespace VulkanContext {

    PipeLineLayout::PipeLineLayout(const VulkanDevice &_device) : 
        device (_device) 
    {
        
    }

    PipeLineLayout::~PipeLineLayout()
    {
        if (handle != VK_NULL_HANDLE)
            vkDestroyPipelineLayout(device.logicalDevice, handle, nullptr);
    }

    void PipeLineLayout::createPipeLineLayout()
    {
        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

        if (vkCreatePipelineLayout(device.logicalDevice, &pipelineLayoutInfo, nullptr, &handle) != VK_SUCCESS) {
            throw std::runtime_error("createGraphicPipeline: pipelineLayoutCreate: failed to create pipeline layout!");
        }
    }

    PipeLine::PipeLine(const VulkanDevice &_device)
            : device (_device)
    {

    }
    PipeLine::~PipeLine() {
        if (handle != VK_NULL_HANDLE)
            vkDestroyPipeline(device.logicalDevice, handle, nullptr);
    }

    void PipeLine::createPipeLine(const VkGraphicsPipelineCreateInfo &pipelineInfo) {
        VK_CHECK_RESULT (vkCreateGraphicsPipelines(device.logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &handle));
    }
}
}