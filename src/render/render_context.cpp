#include "render_context.hpp"
#include "render_task.hpp"
#include "render_session.hpp"

#include "vulkan/logical_device.hpp"
#include "vulkan/physical_device.hpp"
#include "vulkan/instance.hpp"
#include "vulkan/swapchain.hpp"

#include "../window/window.hpp"

namespace cruelEngine {
namespace cruelRender {

    RenderContext::RenderContext (RenderProp properties) :
        properties {properties}
    {
        // create Instance.
        glfwInit();
        instance = std::make_unique<Instance>(properties.appInfo, properties.validation, properties.validationLayers, properties.enabledInstanceExtensions);
        glfwTerminate();
        std::cout << "[RenderContext] Instance created" << std::endl;
        
        // pick suitable physical device aka gpu.
        for (auto &gpu : instance->get_gpus())
        {
            if (isPhysicalDeviceSuitable(gpu, properties.features, properties.enabledDeviceExtensions, properties.flags))
            {
                physicalDevice = std::make_unique<PhysicalDevice>(gpu, properties.features);
            }
        }
        std::cout << "[RenderContext] Physical device created" << std::endl;

        // Create Logical device
        device = std::make_unique<LogicalDevice>(*physicalDevice, properties.validationLayers, properties.enabledDeviceExtensions, properties.flags);
        std::cout << "[RenderContext] Logical device created" << std::endl;
    }

    void RenderContext::prepareRender()
    {

    }

    RenderContext::~RenderContext()
    {
        sessions.clear();

        device.reset();

        physicalDevice.reset();

        instance.reset();
    }

    RenderSession& RenderContext::get_session(u32 index) const 
    {
        if (index < sessions.size())
            return *(sessions[index]);
        else {
            std::cout << "[RenderContext] Get render session: out of range" << std::endl;
            return *(sessions[0]);
        }
    }

    void RenderContext::add_session(std::string session_name, SessionProp &properties)
    {
        sessions.push_back(std::make_unique<RenderSession>(*instance, *device, properties));
        std::cout << "[RenderContext] Render Session created : " << session_name << std::endl;
    }

    void RenderContext::draw()
    {
        for (auto &session: sessions)
        {
            session->draw();
        }
    }

    void RenderContext::render_frame()
    {
        // std::cout << "session num: " << sessions.size() << std::endl;
        for (auto &session: sessions)
        {
            if (session->is_session_alive())
                session->render_frame();
        }
    }

    bool RenderContext::is_context_alive()
    {
        vkDeviceWaitIdle(device->get_handle());
        sessions.erase(std::remove_if(sessions.begin(), sessions.end(),
            [](std::unique_ptr<RenderSession> const &session) {return !session->is_session_alive();}
            ),
            sessions.end());
        vkDeviceWaitIdle(device->get_handle());
        return (sessions.size() > 0);
    }

    // void RenderContext::loadObject(const cruelEngine::cruelScene::Object &theObject)
    // {

    // }

    // void RenderContext::loadScene(const cruelEngine::cruelScene::Scene &theScene)
    // {

    // }
}
}