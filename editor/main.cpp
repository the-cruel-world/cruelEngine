#include "../src/common.h"
#include "../src/types.h"
#include <GLFW/glfw3.h>

#include "../src/window/window.hpp"
#include "../src/application/application.hpp"

#include "../src/render/vulkan/instance.hpp"
#include "../src/render/vulkan/render.hpp"
#include "../src/render/vulkan/vkDevice.hpp"
#include "../src/render/vulkan/swapChain.hpp"
#include "../src/render/vulkan/renderPass.hpp"
#include "../src/render/vulkan/shader.hpp"
#include "../src/render/vulkan/pipeLine.hpp"
#include "../src/render/vulkan/buffer.hpp"

#include "../src/debug/debug.hpp"

using namespace cruelEngine;

class Editor : Application{
public:
    Editor () : mainWindow (mainWindowProp), 
        instance (mainWindow, appInfo, settings.validation, settings.validationLayers, settings.enabledInstanceExtensions), 
        device (settings.validation, settings.validationLayers, instance.instance, instance.surface, settings.enabledDeviceExtensions),
        swapChain (instance, device), renderPass (instance, device, swapChain), pipeLine(device, swapChain, renderPass),
        frameBuffer (device, swapChain, renderPass), 
        commandBuffer(device, swapChain, renderPass, pipeLine, frameBuffer)
    {
        std::cout << "[Editor] Hello World" << std::endl;
    };
    ~Editor () {
        destroySemaphores();
        // cleanUp();
    }

    void createSemaphores() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        imagesInFlight.resize(swapChain.images.size(), VK_NULL_HANDLE);

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
        vkAcquireNextImageKHR(device.logicalDevice, swapChain.swapChain, UINT64_MAX, 
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
        submitInfo.pCommandBuffers = &commandBuffer.commandBuffers[imageIndex];

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
        VkSwapchainKHR swapChains[] = {swapChain.swapChain};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr; // Optional

        vkQueuePresentKHR(device.getPresentQueue(), &presentInfo);

        vkQueueWaitIdle(device.getPresentQueue());
        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void init() {
        std::string appHead("[Editor] ");
        std::cout << appHead+"Window initialized!" << std::endl;
        std::cout << appHead+"Vulkan Instance Created!" << std::endl;
        std::cout << appHead+"Suitable GPUS: " << device.suitablePhysicalDevices.size() << std::endl;

        device.selectPhysicalDevice(1);
        std::cout << appHead+"Physical Device Selected!" << std::endl;

        device.createLogicalDevice();
        std::cout << appHead+"Logical Device Created!" << std::endl;

        swapChain.createSwapChain();
        std::cout << appHead+"SwapChain Established!" << std::endl;

        swapChain.createImages();
        std::cout << appHead+"SwapChain Images Created!" << std::endl;

        swapChain.createImageViews();
        std::cout << appHead+"SwapChain Image Views Created!" << std::endl;

        renderPass.createRenderPass();
        std::cout << appHead+"RenderPass Created!" << std::endl;

        pipeLine.createPipiLineALayout();
        std::cout << appHead+"PipeLine Created!" << std::endl;

        frameBuffer.createFrameBuffer();
        std::cout << appHead+"FrameBuffer Created!" << std::endl;

        commandBuffer.createCommandPool();
        std::cout << appHead+"CommandPool Created!" << std::endl;

        commandBuffer.createCommandBuffer();
        std::cout << appHead+"CommandBuffer Created!" << std::endl;

        createSemaphores();
    }

public:

    RenderProp      settings = {true, {"VK_LAYER_KHRONOS_validation"}, {}, {VK_KHR_SWAPCHAIN_EXTENSION_NAME}, false, false};

    VkApplicationInfo           appInfo = {
    .sType=VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pApplicationName = "Editor",
    .pEngineName = "Real Engine",
    .apiVersion = VK_API_VERSION_1_0};

    cruelEngine::WindowProp                  mainWindowProp = {"Editor", 1280, 720, false};
    cruelEngine::Window                      mainWindow;

    cruelEngine::Instance        instance;
    VkPhysicalDeviceFeatures    features = {};
    cruelEngine::VulkanDevice    device;
    cruelEngine::SwapChain       swapChain;
    cruelEngine::RenderPass      renderPass;
    cruelEngine::PipeLine        pipeLine;
    cruelEngine::FrameBuffer     frameBuffer;
    cruelEngine::CommandBuffer   commandBuffer;


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
            usleep(1e5);
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
    Editor mainEditor;
    mainEditor.init();
    std::string appHead("[Editor] ");
    std::cout << appHead+"Entering main loop!" << std::endl;
    mainEditor.run();
    std::cout << appHead+"Exit main loop!" << std::endl;

    return 0;
}
