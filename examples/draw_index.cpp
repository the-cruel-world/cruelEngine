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

class Draw_Index : Application{
public:
    Draw_Index () : mainWindow (mainWindowProp), 
        instance (mainWindow, appInfo, settings.validation, settings.validationLayers, settings.enabledInstanceExtensions), 
        device (settings.validation, settings.validationLayers, instance.instance, instance.surface, settings.enabledDeviceExtensions),
        swapChain (instance, device), renderPass (device), pipeLineLayout(device), pipeLine(device), commandPool(device)
    {
        std::cout << "[Draw_Index] Hello World" << std::endl;
    };
    ~Draw_Index () {
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
        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(device.logicalDevice, swapChain.get_handle(), UINT64_MAX, 
                imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            windowFrameBufferResized();
            return ;
        } else if (result != VK_SUCCESS)
        {
            std::cout << "vkAcquireNextImageKHR Failed" << std::endl;
            glfwSetWindowShouldClose(mainWindow.window, 1);
            return ;
        }

        if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
            vkWaitForFences(device.logicalDevice, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
        }
        // Mark the image as now being in use by this frame
        imagesInFlight[imageIndex] = inFlightFences[currentFrame];
        // std::cout << "Submit" << std::endl;

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

        auto bindingDescription = Vertex::getBindingDescription();
        auto attributeDescriptions = Vertex::getAttributeDescriptions();

        VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 1;
        vertexInputInfo.pVertexBindingDescriptions = &bindingDescription; // Optional
        vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
        vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data(); // Optional

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

            VkBuffer vertexBuffers[] = {vertexBuffer};
            VkDeviceSize offsets[] = {0};
            vkCmdBindVertexBuffers(commandBuffers[i]->get_handle(), 0, 1, vertexBuffers, offsets);
            // vkCmdBindVertexBuffers(commandBuffers[i]->get_handle(), 0, 1, vertexBuffers, offsets);
            vkCmdBindIndexBuffer(commandBuffers[i]->get_handle(), indexBuffer, 0, VK_INDEX_TYPE_UINT16);

            vkCmdDrawIndexed(commandBuffers[i]->get_handle(), static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);

            // vkCmdDraw(commandBuffers[i]->get_handle(), static_cast<uint32_t>(vertices.size()), 1, 0, 0);

            // vkCmdDraw(commandBuffers[i]->get_handle(), 3, 1, 0, 0);

            commandBuffers[i]->end_renderpass();

            commandBuffers[i]->end();
        }
    }

    void windowFrameBufferResized()
    {
        if (clock() - resize_time < window_resize_wait_time )
            return;
        int width = 0, height = 0;
        glfwGetFramebufferSize(mainWindow.window, &width, &height);
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(mainWindow.window, &width, &height);
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(device.logicalDevice);

        // Update SwapChain
        swapChain.update(VkExtent2D({(u32)width, (u32)height}));

        renderPass.update(swapChain.get_colorFormat());

        pipeLine.__destroy();
        preparePipeline();

        frameBuffers.clear();
        prepareFrameBuffer();

        commandBuffers.clear();
        prepareCommandBuffer();

        std::cout << "SwapChain, RenderPass, PipeLine, FrameBuffer, CommandBuffer recreated!" << std::endl;
    }

    void init() {

        glfwSetWindowUserPointer(mainWindow.window, this);
        std::string appHead("[Draw_Index] ");
        std::cout << appHead+"Window initialized!" << std::endl;
        std::cout << appHead+"Vulkan Instance Created!" << std::endl;
        std::cout << appHead+"Suitable GPUS: " << device.suitablePhysicalDevices.size() << std::endl;

        glfwSetFramebufferSizeCallback(mainWindow.window, on_window_resize_cb);

        device.selectPhysicalDevice(1);
        std::cout << appHead+"Physical Device Selected!" << std::endl;

        device.createLogicalDevice();
        std::cout << appHead+"Logical Device Created!" << std::endl;

        swapChain.createSwapChain(swapChainImageCount);
        std::cout << appHead+"SwapChain Established!" << std::endl;

        renderPass.createRenderPass(swapChain.get_colorFormat());
        std::cout << appHead+"RenderPass Created!" << std::endl;

        pipeLineLayout.createPipeLineLayout();
        std::cout << appHead+"PipeLine Layout Created!" << std::endl;

        preparePipeline();
        std::cout << appHead+"PipeLine Created!" << std::endl;

        prepareFrameBuffer();
        std::cout << appHead+"FrameBuffer Created!" << std::endl;

        commandPool.createCommandPool();
        std::cout << appHead+"CommandPool Created!" << std::endl;

        createVertexBuffer();

        createIndexBuffer();

        prepareCommandBuffer();
        std::cout << appHead+"CommandBuffer Created!" << std::endl;

        createSemaphores();

        windowFrameBufferResized();

        last_frame_time = clock();
    }

    static void on_window_resize_cb(GLFWwindow* window, int width, int height)
    {
        // std::cout << "resizedresizedresizedresizedresizedresizedresizedresizedresizedresized" << std::endl;
        auto app = reinterpret_cast<Draw_Index*>(glfwGetWindowUserPointer(window));
        app->resize_time = clock();
    }

    void cleanUp()
    {
        frameBuffers.clear();
        commandBuffers.clear();

        if (vertexBuffer != VK_NULL_HANDLE)
            vkDestroyBuffer(device.logicalDevice, vertexBuffer, nullptr);
        if (vertexBufferMemory != VK_NULL_HANDLE)
            vkFreeMemory(device.logicalDevice, vertexBufferMemory, nullptr);
        if (indexBuffer != VK_NULL_HANDLE)
            vkDestroyBuffer(device.logicalDevice, indexBuffer, nullptr);
        if (indexBufferMemory != VK_NULL_HANDLE)
            vkFreeMemory(device.logicalDevice, indexBufferMemory, nullptr);
    }

    void run (){
        while (!glfwWindowShouldClose(mainWindow.window)){
            if (clock() - last_frame_time < frame_time)
                continue;
            glfwPollEvents();
            drawFrame();
            last_frame_time = clock();
        }
        vkDeviceWaitIdle(device.logicalDevice);
    }

public:

    RenderProp      settings = {true, {"VK_LAYER_KHRONOS_validation"}, {}, {VK_KHR_SWAPCHAIN_EXTENSION_NAME}, false, false};

    VkApplicationInfo           appInfo = {
    .sType=VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pApplicationName = "Draw_Index",
    .pEngineName = "Real Engine",
    .apiVersion = VK_API_VERSION_1_0};

    cruelEngine::WindowProp                  mainWindowProp = {"Draw_Index", 1280, 720, false};
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

    VkBuffer                    vertexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory              vertexBufferMemory = VK_NULL_HANDLE;
    VkBuffer                    indexBuffer = VK_NULL_HANDLE;
    VkDeviceMemory              indexBufferMemory = VK_NULL_HANDLE;


    struct Vertex {
        glm::vec3 pos;
        glm::vec3 color;
        
        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
            return bindingDescription;
        }

        static std::array<VkVertexInputAttributeDescription, 2> getAttributeDescriptions() {
            std::array<VkVertexInputAttributeDescription, 2> attributeDescriptions{};
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);
            attributeDescriptions[1].binding = 0;
            attributeDescriptions[1].location = 1;
            attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[1].offset = offsetof(Vertex, color);
            return attributeDescriptions;
        }
    };

    const std::vector<Vertex> vertices = {
        {{-.5f, -.5f, 0.0f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}},
        {{-.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
        {{0.5f, -.5f, 0.0f}, {0.0f, 1.0f, 0.0f}},
    };

    const std::vector<uint16_t> indices = {
        0, 1, 2, 1, 0, 3
    };

    void createVertexBuffer()
    {
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        // stage buffer
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void *data;
        vkMapMemory(device.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t) bufferSize);
        vkUnmapMemory(device.logicalDevice, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

        copyBuffer(stagingBuffer, vertexBuffer, bufferSize);
        vkDestroyBuffer(device.logicalDevice, stagingBuffer, nullptr);
        vkFreeMemory(device.logicalDevice, stagingBufferMemory, nullptr);
    }

    void createIndexBuffer() {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(device.logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t) bufferSize);
        vkUnmapMemory(device.logicalDevice, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

        copyBuffer(stagingBuffer, indexBuffer, bufferSize);

        vkDestroyBuffer(device.logicalDevice, stagingBuffer, nullptr);
        vkFreeMemory(device.logicalDevice, stagingBufferMemory, nullptr);
    }


    void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = commandPool.get_handle();
        allocInfo.commandBufferCount = 1;

        VkCommandBuffer commandBuffer;
        vkAllocateCommandBuffers(device.logicalDevice, &allocInfo, &commandBuffer);

        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

        vkBeginCommandBuffer(commandBuffer, &beginInfo);

        VkBufferCopy copyRegion{};
        copyRegion.srcOffset = 0; // Optional
        copyRegion.dstOffset = 0; // Optional
        copyRegion.size = size;
        vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);
        vkEndCommandBuffer(commandBuffer);

        VkSubmitInfo submitInfo{};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &commandBuffer;

        vkQueueSubmit(device.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
        vkQueueWaitIdle(device.getGraphicsQueue());
        vkFreeCommandBuffers(device.logicalDevice, commandPool.get_handle(), 1, &commandBuffer);
    }

    void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory) {
        VkBufferCreateInfo bufferInfo{};
        bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
        bufferInfo.size = size;
        bufferInfo.usage = usage;
        bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateBuffer(device.logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to create buffer!");
        }

        VkMemoryRequirements memRequirements;
        vkGetBufferMemoryRequirements(device.logicalDevice, buffer, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(memRequirements.memoryTypeBits, properties);

        if (vkAllocateMemory(device.logicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate buffer memory!");
        }

        vkBindBufferMemory(device.logicalDevice, buffer, bufferMemory, 0);
    }

    uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
        VkPhysicalDeviceMemoryProperties memProperties;
        vkGetPhysicalDeviceMemoryProperties(device.physicalDevice, &memProperties);
        for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
            if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
                return i;
            }
        }
        throw std::runtime_error("failed to find suitable memory type!");
    }

    u32                         swapChainImageCount = 5;
    const int                   MAX_FRAMES_IN_FLIGHT = 2;
    VkSemaphore                 imageAvailableSemaphore;
    VkSemaphore                 renderFinishedSemaphore;
    std::vector<VkSemaphore>    imageAvailableSemaphores;
    std::vector<VkSemaphore>    renderFinishedSemaphores;

    clock_t                     resize_time;
    clock_t                     last_frame_time;
    const clock_t               frame_time = 2e4;
    const clock_t               window_resize_wait_time = frame_time * 5;

    std::vector<VkFence> inFlightFences;
    std::vector<VkFence> imagesInFlight;
    size_t currentFrame = 0;
};

u32 cruelEngine::Window::count = 0;
bool cruelEngine::Window::glfw_inited = false;

int main(int argc, char const *argv[])
{
    Draw_Index mainDraw_Index;
    mainDraw_Index.init();
    std::string appHead("[Draw_Index] ");
    std::cout << appHead+"Entering main loop!" << std::endl;
    mainDraw_Index.run();
    std::cout << appHead+"Exit main loop!" << std::endl;

    return 0;
}
