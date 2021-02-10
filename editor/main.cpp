#include "../src/common.h"
#include "../src/types.h"

#include "../src/application/application.hpp"
#include "../src/window/window.hpp"

#include "../src/render/render.hpp"
#include "../src/render/vkDevice.hpp"
#include "../src/render/instance.hpp"
#include "../src/render/swapChain.hpp"
#include "../src/render/renderPass.hpp"
#include "../src/render/shader.hpp"
#include "../src/render/pipeLine.hpp"
#include "../src/render/buffer.hpp"

#include "../src/debug/debug.hpp"

using namespace realEngine;

class Editor : Application{
public:
    Editor () : mainWindow (mainWindowProp), 
        instance (mainWindow, appInfo, settings.validation, settings.validationLayers, settings.enabledInstanceExtensions), 
        device (settings.validation, settings.validationLayers, instance.instance, instance.surface, settings.enabledDeviceExtensions),
        swapChain (instance, device), renderPass (instance, device, swapChain), pipeLine(device, swapChain, renderPass),
        frameBuffer (device, swapChain, renderPass)
    {
        std::cout << "Hello World" << std::endl;
    };
    ~Editor () {};

    RenderProp      settings = {true, {"VK_LAYER_KHRONOS_validation"}, {}, {VK_KHR_SWAPCHAIN_EXTENSION_NAME}, false, false};

    VkApplicationInfo           appInfo = {
    .sType=VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pApplicationName = "Editor",
    .pEngineName = "Real Engine",
    .apiVersion = VK_API_VERSION_1_0};

    realEngine::WindowProp                  mainWindowProp = {"Editor", 1280, 720, false};
    realEngine::Window                      mainWindow;

    realEngine::Instance        instance;
    VkPhysicalDeviceFeatures    features = {};
    realEngine::VulkanDevice    device;
    realEngine::SwapChain       swapChain;
    realEngine::RenderPass      renderPass;
    realEngine::PipeLine        pipeLine;
    realEngine::FrameBuffer     frameBuffer;

    void run (){
        assert(mainWindow.window != nullptr);
        while (!glfwWindowShouldClose(mainWindow.window)){
            usleep(1e5);
            glfwPollEvents();
        }
    }
};

u32 realEngine::Window::count = 0;
bool realEngine::Window::glfw_inited = false;

int main(int argc, char const *argv[])
{
    Editor mainEditor;
    std::string appHead("[Editor] ");
    std::cout << appHead+"Window initialized!" << std::endl;
    std::cout << appHead+"Vulkan Instance Created!" << std::endl;
    std::cout << appHead+"Suitable GPUS: " << mainEditor.device.suitablePhysicalDevices.size() << std::endl;

    mainEditor.device.selectPhysicalDevice(1);
    std::cout << appHead+"Physical Device Selected!" << std::endl;

    mainEditor.device.createLogicalDevice();
    std::cout << appHead+"Logical Device Created!" << std::endl;

    mainEditor.swapChain.createSwapChain();
    std::cout << appHead+"SwapChain Established!" << std::endl;

    mainEditor.swapChain.createImages();
    std::cout << appHead+"SwapChain Images Created!" << std::endl;

    mainEditor.swapChain.createImageViews();
    std::cout << appHead+"SwapChain Image Views Created!" << std::endl;

    mainEditor.renderPass.createRenderPass();
    std::cout << appHead+"RenderPass Created!" << std::endl;

    mainEditor.pipeLine.createPipiLineALayout();
    std::cout << appHead+"PipeLine Created!" << std::endl;

    mainEditor.frameBuffer.createFrameBuffer();
    std::cout << appHead+"FrameBuffer Created!" << std::endl;

    std::cout << appHead+"Entering main loop!" << std::endl;
    mainEditor.run();
    std::cout << appHead+"Exit main loop!" << std::endl;

    return 0;
}
