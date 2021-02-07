#include "../src/common.h"
#include "../src/types.h"

#include "../src/application/application.hpp"
#include "../src/window/window.hpp"

#include "../src/render/render.hpp"
#include "../src/render/vkDevice.hpp"
#include "../src/render/instance.hpp"

#include "../src/debug/debug.hpp"

using namespace realEngine;

class Editor : Application{
public:
    Editor () : mainWindow(mainWindowProp), 
        instance(mainWindow, appInfo, settings.validation, settings.validationLayers, settings.enabledInstanceExtensions), 
        device (settings.validation, settings.validationLayers, instance.instance, instance.surface, settings.enabledDeviceExtensions) 
    {
        std::cout << "Hello World" << std::endl;
    };
    ~Editor () {};

    RenderProp      settings = {true, {"VK_LAYER_KHRONOS_validation"}, {}, {}, false, false};

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

    void run (){
        assert(mainWindow.window != nullptr);
        while (!glfwWindowShouldClose(mainWindow.window)){
            glfwPollEvents();
        }
    }
};

u32 realEngine::Window::count = 0;
bool realEngine::Window::glfw_inited = false;

int main(int argc, char const *argv[])
{
    Editor mainEditor;
    std::cout << "Suitable GPUS: " << mainEditor.device.suitablePhysicalDevices.size() << std::endl;
    mainEditor.device.selectPhysicalDevice(1);
    mainEditor.device.createLogicalDevice();
    mainEditor.run();

    return 0;
}
