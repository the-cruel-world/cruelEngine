#include "../src/common.h"
#include "../src/types.h"
#include <GLFW/glfw3.h>
#include <memory>

#include "../src/window/window.hpp"
#include "../src/application/application.hpp"

#include "../src/render/vulkan/instance.hpp"
#include "../src/render/vulkan/render.hpp"
#include "../src/render/vulkan/vkDevice.hpp"
#include "../src/render/vulkan/swapChain.hpp"
#include "../src/render/vulkan/renderPass.hpp"
#include "../src/render/vulkan/shader.hpp"
#include "../src/render/vulkan/pipeLine.hpp"
#include "../src/render/vulkan/frameBuffer.hpp"
#include "../src/render/vulkan/commandBuffer.hpp"

#include "../src/debug/debug.hpp"

using namespace cruelEngine;

using namespace cruelEngine::VulkanContext;

class Triangle : Application{
public:
    Triangle () : mainWindow (mainWindowProp), 
        instance (mainWindow, appInfo, settings.validation, settings.validationLayers, settings.enabledInstanceExtensions), 
        device (settings.validation, settings.validationLayers, instance.instance, instance.surface, settings.enabledDeviceExtensions),
        swapChain (instance, device), renderPass (device, swapChain), pipeLineLayout(device), pipeLine(device), commandPool(device)
    {
        std::cout << "[Triangle] Hello World" << std::endl;
    };
    ~Triangle () {
        destroySemaphores();
        cleanUp();
    }

    void createSemaphores() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        imagesInFlight.resize(swapChain.get_imageViews().size(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(device.logicalDevice, &semaphoreInfo, nullptr, 
                        &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                    vkCreateSemaphore(device.logicalDevice, &semaphoreInfo, nullptr, 
                        &renderFinishedSemaphores[i]) != VK_SUCCESS || 
                    vkCreateFence(device.logicalDevice, &fenceInfo, nullptr, 
                        &inFlightFences[i]) != VK_SUCCESS)
                throw std::runtime_error("createSemaphores: failed to create semaphores!");
        }
    }

    void destroySemaphores() {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(device.logicalDevice, renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device.logicalDevice, imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device.logicalDevice, inFlightFences[i], nullptr);
        }
    }

    void drawFrame()
    {
        vkWaitForFences(device.logicalDevice, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
        vkResetFences(device.logicalDevice, 1, &inFlightFences[currentFrame]);

        uint32_t imageIndex;
        vkAcquireNextImageKHR(device.logicalDevice, swapChain.get_handle(), UINT64_MAX, 
                imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(device.logicalDevice, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
        // Mark the image as now being in use by this frame
        imagesInFlight[imageIndex] = inFlightFences[currentFrame];


        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffers[imageIndex]->get_handle();

        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        vkResetFences(device.logicalDevice, 1, &inFlightFences[currentFrame]);
        if (vkQueueSubmit(device.getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
            throw std::runtime_error("failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        VkSwapchainKHR swapChains[] = {swapChain.get_handle()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional

        vkQueuePresentKHR(device.getPresentQueue(), &presentInfo);

        vkQueueWaitIdle(device.getPresentQueue());
        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void preparePipeline()
    {
        Shader vertShader (device.logicalDevice, "../data/shaders/triangle/vert.spv");
        Shader fragShader (device.logicalDevice, "../data/shaders/triangle/frag.spv");

        VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShader.shader;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShader.shader;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = {
            vertShaderStageInfo, 
            fragShaderStageInfo
        };

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

        VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (float) swapChain.get_extent().width;
        viewport.height = (float) swapChain.get_extent().height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = {0, 0};
        scissor.extent = swapChain.get_extent();

        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer{};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f; // Optional
        rasterizer.depthBiasClamp = 0.0f; // Optional
        rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

        VkPipelineMultisampleStateCreateInfo multisampling{};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisampling.minSampleShading = 1.0f; // Optional
        multisampling.pSampleMask = nullptr; // Optional
        multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
        multisampling.alphaToOneEnable = VK_FALSE; // Optional

        VkPipelineColorBlendAttachmentState colorBlendAttachment{};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

        VkPipelineColorBlendStateCreateInfo colorBlending{};
        colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlending.logicOpEnable = VK_FALSE;
        colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
        colorBlending.attachmentCount = 1;
        colorBlending.pAttachments = &colorBlendAttachment;
        colorBlending.blendConstants[0] = 0.0f; // Optional
        colorBlending.blendConstants[1] = 0.0f; // Optional
        colorBlending.blendConstants[2] = 0.0f; // Optional
        colorBlending.blendConstants[3] = 0.0f; // Optional

        VkDynamicState dynamicStates[] = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_LINE_WIDTH
        };

        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = 2;
        dynamicState.pDynamicStates = dynamicStates;

        VkGraphicsPipelineCreateInfo pipelineInfo{};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;

        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = nullptr; // Optional
        pipelineInfo.pColorBlendState = &colorBlending;
        pipelineInfo.pDynamicState = nullptr; // Optional
        pipelineInfo.layout = pipeLineLayout.get_handle();
        pipelineInfo.renderPass = renderPass.get_handle();
        pipelineInfo.subpass = 0;

        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
        pipelineInfo.basePipelineIndex = -1; // Optional

        pipeLine.createPipeLine(pipelineInfo);
    }

    void prepareFrameBuffer()
    {
        for (auto &view: swapChain.get_imageViews())
        {
            frameBuffers.push_back(std::make_shared<cruelEngine::VulkanContext::FrameBuffer>(device, view, swapChain.get_extent(), renderPass.get_handle()));
        }
    }

    void prepareCommandBuffer()
    {
        /*! It's an ugly implementation. Should be optimized later. */
        for (size_t i = 0; i < frameBuffers.size(); i++)
        {
            commandBuffers.push_back(std::make_shared<cruelEngine::VulkanContext::CommandBuffer>(commandPool, VK_COMMAND_BUFFER_LEVEL_PRIMARY));
        }

        for (size_t i = 0; i < commandBuffers.size(); i++)
        {
            // std::cout << "Record cmd[" << i << "] id: " << commandBuffers[i]->get_handle() << std::endl;
            commandBuffers[i]->begin();

            commandBuffers[i]->begin_renderpass(renderPass.get_handle(), frameBuffers[i]->get_handle(), swapChain.get_extent());

            vkCmdBindPipeline(commandBuffers[i]->get_handle(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeLine.get_handle());

            vkCmdDraw(commandBuffers[i]->get_handle(), 3, 1, 0, 0);

            commandBuffers[i]->end_renderpass();

            commandBuffers[i]->end();
        }
    }

    void init() {
        std::string appHead("[Triangle] ");
        std::cout << appHead+"Window initialized!" << std::endl;
        std::cout << appHead+"Vulkan Instance Created!" << std::endl;
        std::cout << appHead+"Suitable GPUS: " << device.suitablePhysicalDevices.size() << std::endl;

        device.selectPhysicalDevice(1);
        std::cout << appHead+"Physical Device Selected!" << std::endl;

        device.createLogicalDevice();
        std::cout << appHead+"Logical Device Created!" << std::endl;

        swapChain.createSwapChain(swapChainImageCount);
        std::cout << appHead+"SwapChain Established!" << std::endl;

        swapChain.createImages();
        std::cout << appHead+"SwapChain Images Created!" << std::endl;

        swapChain.createImageViews();
        std::cout << appHead+"SwapChain Image Views Created!" << std::endl;

        renderPass.createRenderPass();
        std::cout << appHead+"RenderPass Created!" << std::endl;

        pipeLineLayout.createPipeLineLayout();
        std::cout << appHead+"PipeLine Layout Created!" << std::endl;

        preparePipeline();
        std::cout << appHead+"PipeLine Created!" << std::endl;

        prepareFrameBuffer();
        std::cout << appHead+"FrameBuffer Created!" << std::endl;

        commandPool.createCommandPool();
        std::cout << appHead+"CommandPool Created!" << std::endl;

        prepareCommandBuffer();
        std::cout << appHead+"CommandBuffer Created!" << std::endl;

        createSemaphores();
    }

    void cleanUp()
    {
        frameBuffers.clear();
        commandBuffers.clear();
    }

public:

    RenderProp      settings = {true, {"VK_LAYER_KHRONOS_validation"}, {}, {VK_KHR_SWAPCHAIN_EXTENSION_NAME}, false, false};

    VkApplicationInfo           appInfo = {
    .sType=VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pApplicationName = "Triangle",
    .pEngineName = "Real Engine",
    .apiVersion = VK_API_VERSION_1_0};

    cruelEngine::WindowProp                  mainWindowProp = {"Triangle", 1280, 720, false};
    cruelEngine::Window                      mainWindow;

    cruelEngine::VulkanContext::Instance        instance;
    VkPhysicalDeviceFeatures    features = {};
    cruelEngine::VulkanContext::VulkanDevice    device;
    cruelEngine::VulkanContext::SwapChain       swapChain;
    cruelEngine::VulkanContext::RenderPass      renderPass;
    cruelEngine::VulkanContext::PipeLineLayout  pipeLineLayout;
    cruelEngine::VulkanContext::PipeLine        pipeLine;
    cruelEngine::VulkanContext::CommandPool     commandPool;
    std::vector<std::shared_ptr<cruelEngine::VulkanContext::FrameBuffer>>   frameBuffers {};
    std::vector<std::shared_ptr<cruelEngine::VulkanContext::CommandBuffer>> commandBuffers {};

    u32                         swapChainImageCount = 5;
    const int                   MAX_FRAMES_IN_FLIGHT = 2;
    VkSemaphore                 imageAvailableSemaphore;
    VkSemaphore                 renderFinishedSemaphore;
    std::vector<VkSemaphore>    imageAvailableSemaphores;
    std::vector<VkSemaphore>    renderFinishedSemaphores;

    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;

    void run (){
        assert(mainWindow.window != nullptr);
        while (!glfwWindowShouldClose(mainWindow.window)){
            usleep(1e4);
            glfwPollEvents();
            drawFrame();
        }
        vkDeviceWaitIdle(device.logicalDevice);
    }
};

u32 cruelEngine::Window::count = 0;
bool cruelEngine::Window::glfw_inited = false;

int main(int argc, char const *argv[])
{
    Triangle mainTriangle;
    mainTriangle.init();
    std::string appHead("[Triangle] ");
    std::cout << appHead+"Entering main loop!" << std::endl;
    mainTriangle.run();
    std::cout << appHead+"Exit main loop!" << std::endl;

    return 0;
}
