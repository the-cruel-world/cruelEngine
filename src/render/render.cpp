#include "render.hpp"

// Engine::Engine()
// {
//     std::cout << "Hello World" << std::endl;
// }

// void    Engine::run()
// {
//     initWindow();
//     initVulkan();
//     renderLoop();
// }

// void    Engine::initVulkan()
// { 
//     createInstance();
//     createWindowSurface();
//     pickPhysicalDevice();
//     createLogicalDevice();
// }

// Engine::~Engine()
// {
//     destroyLogicalDevice();
//     destroyWindowSurface();
//     destroyInstance();
//     destroyWindow();
// }

// void    Engine::initWindow()
// {
//     glfwInit();
//     glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
//     if (settings.fullscreen)
//         mainWindow = glfwCreateWindow(windowWidth, windowHeight, title.c_str(),
//             glfwGetPrimaryMonitor(), nullptr);
//     else
//         mainWindow = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), nullptr, nullptr);
//     if (!mainWindow) {
//         throw std::runtime_error("Engine: initWindow(): Failed to create glfw window");
//         exit(-1);
//     }
//     glfwSetWindowUserPointer(mainWindow, this);
//     std::cout << "initWindow(): Create Window" << std::endl;
// }

// void    Engine::destroyWindow()
// {
//     glfwDestroyWindow(mainWindow);
//     glfwTerminate();
//     std::cout << "destroyWindow(): destroy Window" << std::endl;
// }

// void    Engine::createWindowSurface()
// {
//     VK_CHECK_RESULT(glfwCreateWindowSurface(vkInstance, mainWindow
//         , nullptr, &surface));
//     std::cout << "createWindowSurface(): create Window Surface" << std::endl;
// }

// void    Engine::destroyWindowSurface()
// {
//     vkDestroySurfaceKHR(vkInstance, surface, nullptr);
//     std::cout << "destroyWindowSurface(): destroy Window Surface" << std::endl;
// }

// void Engine::createLogicalDevice()
// {
//     vulkanDevice = new VulkanDevice(physicalDevice);
//     VK_CHECK_RESULT(vulkanDevice->createLogicalDevice({},settings.enabledDeviceExtensions, deviceCreatepNextChain));
//     device = vulkanDevice->logicalDevice;
//     assert (device != nullptr);
//     std::cout << "createLogicalDevice(): create Logical Device" << std::endl;
// }

// void Engine::destroyLogicalDevice()
// {
//     delete vulkanDevice;
//     vulkanDevice = nullptr;
//     std::cout << "destroyLogicalDevice(): destroy Logical Device" << std::endl;
// }

// void    Engine::renderLoop(){
//     while(!glfwWindowShouldClose(mainWindow))
//     {
//         usleep(1e5);
//         glfwPollEvents();
//     }
// }

// void    Engine::createInstance()
// {
//     VkApplicationInfo appInfo = {};
//     appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
//     appInfo.pEngineName = name.c_str();
//     appInfo.engineVersion = REAL_ENGINE_VERSION;
//     appInfo.pApplicationName = name.c_str();
//     appInfo.apiVersion = vkApiVersion;

//     std::vector<const char*> instanceExtensions = getRequiredInstanceExtensions();

//     VkInstanceCreateInfo instCreateInfo = {};
//     instCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
//     instCreateInfo.pNext = NULL;
//     instCreateInfo.pApplicationInfo = &appInfo;

//     instCreateInfo.enabledExtensionCount = static_cast<u32> (instanceExtensions.size());
//     instCreateInfo.ppEnabledExtensionNames = instanceExtensions.data();

//     if (settings.validation) {
//         instCreateInfo.enabledLayerCount = static_cast<u32> (settings.validationLayers.size());
//         instCreateInfo.ppEnabledLayerNames = settings.validationLayers.data();
//     } else 
//         instCreateInfo.enabledLayerCount = 0;

//     VK_CHECK_RESULT(vkCreateInstance(&instCreateInfo, nullptr, &vkInstance));
// }
// void    Engine::destroyInstance()
// {
//     vkDestroyInstance(this->vkInstance, nullptr);
// }

// //int     Engine::render(){ return 0;}
// //void    Engine::viewChanged(){}
// //void    Engine::keyPressed(){}
// //void    Engine::moseMoved(){}
// //void    Engine::windowResized(){}

// //int     Engine::createCommandBuffers(){}
// //int     Engine::setDetphStencil(){}
// //int     Engine::setFrameBuffers(){}
// //int     Engine::setRenderPass(){}

// //int     Engine::getEnabledFearures(){}

// std::vector<const char*>
//         Engine::getRequiredInstanceExtensions()
// {
//     std::vector<const char*> extensions;

// 	unsigned int glfwExtensionCount = 0;
// 	const char** glfwExtensions;
// 	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

// 	for (unsigned int i = 0; i < glfwExtensionCount; i++) {
// 		extensions.push_back(glfwExtensions[i]);
// 	}
//     if (settings.enabledInstanceExtensions.size() > 0) {
//         for (const char * enabled: settings.enabledInstanceExtensions)
//             extensions.push_back(enabled);
//     }
// 	if (settings.validation) {
// 		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
// 	}
// 	return extensions;
// }

// bool Engine::isDeviceSuitable(VkPhysicalDevice device){
//     // Queue family support 
//     u32 queueFamilyCount = 0;
//     vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
//     std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
//     vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

//     VkBool32 graphicSupport = 0;
//     VkBool32 surfaceSupport = 0;
//     int i=0;
//     for (const auto &queueFamily: queueFamilies) {
//         if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
//             graphicSupport = true;
//         vkGetPhysicalDeviceSurfaceSupportKHR(device, i, this->surface, &surfaceSupport);

//         if (graphicSupport && surfaceSupport)
//             break;
//         i ++;
//     }
//     if (!graphicSupport || !surfaceSupport)
//         return false;
//     // Extensions support
//     u32 extensionCount = 0;
//     vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
//     std::vector<VkExtensionProperties> availableExtensions(extensionCount);
//     vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);
//     std::set<std::string> requiredExtensions(settings.enabledDeviceExtensions.begin(),
//         settings.enabledDeviceExtensions.end());
//     for (const auto& extension: availableExtensions) {
//         requiredExtensions.erase(extension.extensionName);
//     }
//     if (!requiredExtensions.empty())
//         return false;
//     // swapchain support
//     VkSurfaceCapabilitiesKHR cap;
//     vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, this->surface, &cap);

//     u32 formatCount = 0;
//     vkGetPhysicalDeviceSurfaceFormatsKHR(device, this->surface, &formatCount, nullptr);
//     if (formatCount == 0)
//         return false;

//     u32 presentModCount = 0;
//     vkGetPhysicalDeviceSurfacePresentModesKHR(device, this->surface, &presentModCount, nullptr);
//     if (presentModCount == 0)
//         return false;
//     return true;
// }

// void Engine::pickPhysicalDevice(){
//     u32 deviceCount = 0;
//     vkEnumeratePhysicalDevices(this->vkInstance, &deviceCount, nullptr);

//     if (!deviceCount)
//         throw std::runtime_error("Failed to find GPUS with Vulkan support.");
    
//     std::vector<VkPhysicalDevice> physicalDevices(deviceCount);
//     vkEnumeratePhysicalDevices(this->vkInstance, &deviceCount, physicalDevices.data());

//     for (const auto &physicalDev: physicalDevices) {
//         if (isDeviceSuitable(physicalDev)) {
//             this->physicalDevice = physicalDev;
//             break;
//         }
//     }
//     if (physicalDevice == VK_NULL_HANDLE)
//         throw std::runtime_error("No suitable GPU found!");
// }