#pragma once

#include "../common.h"
#include "../render/vulkan/vkcommon.h"
#include "../types.h"

#include "../../external/imgui/imgui.h"

namespace cruelEngine {

namespace VulkanContext {
    class LogicalDevice;
}

    class Gui {
    public:
        Gui (const VulkanContext::LogicalDevice& _device);
        ~Gui() {};

        void draw();

        void newFrame() { ImGui::NewFrame();}

        void prepare_pipeline();
        void prepare_resource();
        void free_resource();
        void destroy_pipeline();

    private:
        const VulkanContext::LogicalDevice &device;
        VkDescriptorPool                descriptorPool = VK_NULL_HANDLE;
		VkDescriptorSetLayout           descriptorSetLayout = VK_NULL_HANDLE;
		VkDescriptorSet                 descriptorSet = VK_NULL_HANDLE;
		VkPipelineLayout                pipelineLayout = VK_NULL_HANDLE;
		VkPipeline                      pipeline = VK_NULL_HANDLE;

        VkDeviceMemory                  fontMemory = VK_NULL_HANDLE;
		VkImage                         fontImage = VK_NULL_HANDLE;
		VkImageView                     fontView = VK_NULL_HANDLE;
		VkSampler                       sampler = VK_NULL_HANDLE;

        float                           scale = 0.5;
        bool                            visible = true;
		bool                            updated = false;


    };
}