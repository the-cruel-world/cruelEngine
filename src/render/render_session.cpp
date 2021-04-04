#include "render_session.hpp"
#include "gui_overlay.hpp"
#include "render_task.hpp"

#include "../scene/object.hpp"
#include "../scene/scene.hpp"
#include "render_tasks/render_tasks.hpp"
#include "vulkan/command_buffer.hpp"
#include "vulkan/command_pool.hpp"
#include "vulkan/frame_buffer.hpp"
#include "vulkan/instance.hpp"
#include "vulkan/logical_device.hpp"
#include "vulkan/queue.hpp"
#include "vulkan/render_pass.hpp"
#include "vulkan/swapchain.hpp"

namespace cruelEngine::cruelRender
{
RenderSession::RenderSession(Instance &instance, LogicalDevice &device,
                             SessionProp &session_properties) :
    instance{instance}, device{device}, session_properties{session_properties}
{
    //! Create a new window for this session.
    window                    = std::make_unique<Window>(session_properties.window_prop);
    on_window_resize_callback = on_window_resize_cb;
    glfwSetFramebufferSizeCallback(&window->get_handle(), on_window_resize_callback);

    //! create a render surface for this session.
    VK_CHECK_RESULT(
        glfwCreateWindowSurface(instance.get_handle(), &(window->get_handle()), nullptr, &surface));

    VkExtent2D extent = {};

    graphic_queue = &device.get_suitable_graphics_queue(0);

    present_queue = &device.get_suitable_present_queue(surface, 1);

#ifdef RENDER_DEBUG
    std::cout << "[Rendersession] graphic queue info: " << graphic_queue->get_family_index()
              << graphic_queue->get_index() << std::endl;
    std::cout << "[Rendersession] present queue info: " << present_queue->get_family_index()
              << present_queue->get_index() << std::endl;
#endif

    swapchain = std::make_unique<Swapchain>(device, surface, extent, imgCount,
                                            VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
                                            VK_PRESENT_MODE_FIFO_KHR);
#ifdef RENDER_DEBUG
    std::cout << "[Rendersession] swapchain creatd!" << std::endl;
#endif

    prepare_render_pass();
    // std::cout << "[Rendersession] renderpass creatd!" << std::endl;

    for (auto &image_view : swapchain->get_imageViews())
    {
        frameBuffer.push_back(std::make_unique<FrameBuffer>(
            device, image_view, swapchain->get_properties().extent, *render_pass));
    }
#ifdef RENDER_DEBUG
    std::cout << "[Rendersession] framebuffers creatd!" << std::endl;
#endif
    createSemaphores();

#ifdef RENDER_DEBUG
    std::cout << "[Rendersession] semaphores creatd!" << std::endl;
#endif

    commandPool = std::make_unique<CommandPool>(device, graphic_queue->get_family_index(),
                                                CommandBuffer::ResetModeFlags::ResetIndividually);

    for (size_t i = 0; i < imgCount; i++)
    {
        commandBuffers.push_back(
            commandPool->RequestCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY));
    }

#ifdef RENDER_DEBUG
    commandPool->test_list_commands();
    std::cout << "[Rendersession] commandbuffers creatd!" << std::endl;
#endif

    // Init time_markers
    frame_time_marker  = std::chrono::high_resolution_clock::now();
    render_time_marker = std::chrono::high_resolution_clock::now();
}

void on_window_resize_cb(GLFWwindow *window, int width, int height)
{
    // std::cout <<
    // "resizedresizedresizedresizedresizedresizedresizedresizedresized"
    //              "resized"
    // << std::endl;
    // auto app = reinterpret_cast<Texture*>(glfwGetWindowUserPointer(window));
    // app->resize_time = clock();
}

RenderSession::~RenderSession()
{
    //! run destructor until all the render process ends.
    device.wait_idle();

    commandBuffers.clear();
    commandPool->ResetPool();
    commandPool.reset();

    device.get_commandPool().ResetPool();

    tasks.clear();
    guiOverlay.reset();

    destroySemaphores();
    frameBuffer.clear();
    render_pass.reset();
    swapchain.reset();
#ifdef RENDER_DEBUG
    std::cout << "[RenderSession::destructor] destroy guiOverlay finally" << std::endl;
#endif
    if (surface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(instance.get_handle(), surface, nullptr);
        surface = VK_NULL_HANDLE;
    }
    window.reset();
#ifdef RENDER_DEBUG
    std::cout << "Session destroied" << std::endl;
#endif
}

void RenderSession::add_new_task(std::unique_ptr<RenderTask> task)
{
    tasks.push_back(std::move(task));
}

void RenderSession::setGuiOverlay(std::shared_ptr<GuiOverlay> gui)
{
    guiOverlay = gui;
}

std::shared_ptr<GuiOverlay> RenderSession::getGuiOverlay()
{
    return guiOverlay;
}

void RenderSession::prepare_render_pass()
{
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format         = swapchain->get_surface_format().format;
    colorAttachment.samples        = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp         = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp        = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout  = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachment.finalLayout    = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout     = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments    = &colorAttachmentRef;

    VkSubpassDependency dependency{};
    dependency.srcSubpass    = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass    = 0;
    dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    RenderPassAttachment attachments = {.colorAttachment    = colorAttachment,
                                        .colorAttachmentRef = colorAttachmentRef,
                                        .subpassDependency  = dependency,
                                        .subpass            = {subpass}};

    render_pass = std::make_unique<RenderPass>(device, attachments);
}

void RenderSession::draw()
{
#ifdef RENDER_DEBUG
    std::cout << "Session tasks: " << tasks.size() << std::endl;
#endif
    if (tasks.size() == 0 && guiOverlay == nullptr)
        return;

    graphic_queue->wait_idle();

    for (size_t i = 0; i < commandBuffers.size(); i++)
    {
        commandBuffers[i].get().begin();
#ifdef RENDER_DEBUG
        std::cout << "[session] cmd begin " << i << std::endl;
#endif
        VkViewport viewport{};
        viewport.x        = 0.0f;
        viewport.y        = 0.0f;
        viewport.width    = static_cast<float>(swapchain.get()->get_extent().width);
        viewport.height   = static_cast<float>(swapchain.get()->get_extent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        commandBuffers[i].get().setViewport(0, {viewport});
        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = swapchain.get()->get_extent();
        commandBuffers[i].get().setScissor(0, {scissor});

        commandBuffers[i].get().begin_renderpass(render_pass->get_handle(),
                                                 frameBuffer[i]->get_handle(),
                                                 swapchain->get_properties().extent);
        for (auto &task : tasks)
        {
            task->draw(commandBuffers[i].get(), i);
        }

        if (guiOverlay != nullptr)
        {
#ifdef RENDER_DEBUG
            std::cout << "[session] draw ui overlay. cmd" << i << std::endl;
#endif
            vkCmdSetViewport(commandBuffers[i].get().get_handle(), 0, 1, &viewport);
            vkCmdSetScissor(commandBuffers[i].get().get_handle(), 0, 1, &scissor);
            guiOverlay->Draw(commandBuffers[i].get());
        }

        commandBuffers[i].get().end_renderpass();
        commandBuffers[i].get().end();
#ifdef RENDER_DEBUG
        std::cout << "[session] cmd end " << i << std::endl;
#endif
    }
}

void RenderSession::render_frame()
{
    render_time_marker = std::chrono::high_resolution_clock::now();
    vkWaitForFences(device.get_handle(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
    vkResetFences(device.get_handle(), 1, &inFlightFences[currentFrame]);

    u32      image_index;
    VkResult result =
        swapchain->acquire_next_image(image_index, imageAvailableSemaphores[currentFrame]);

    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    {
        update_swapchain();
        return;
    }

    if (imagesInFlight[image_index] != VK_NULL_HANDLE)
    {
        vkWaitForFences(device.get_handle(), 1, &imagesInFlight[image_index], VK_TRUE, UINT64_MAX);
    }
    imagesInFlight[image_index] = inFlightFences[currentFrame];

    if (guiOverlay != nullptr)
    {
        guiOverlay->renderFrame();
        if (guiOverlay->needUpdate())
        {
            guiOverlay->update();
#ifdef RENDER_DEBUG
            std::cout << "[Session::render_frame] guiOverlay needs Update." << std::endl;
#endif
            draw();
        }
#ifdef RENDER_DEBUG
        else
            std::cout << "[Session::render_frame] guiOverlay doesn't need Update." << std::endl;
#endif
    }

    for (auto &task : tasks)
    {
        task->render();
    }

    VkSubmitInfo submitInfo{VK_STRUCTURE_TYPE_SUBMIT_INFO};

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers    = &(commandBuffers[image_index].get().get_handle());

    VkSemaphore          waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[]     = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount         = 1;
    submitInfo.pWaitSemaphores            = waitSemaphores;
    submitInfo.pWaitDstStageMask          = waitStages;

    VkSemaphore signalSemaphores[]  = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores    = signalSemaphores;

    vkResetFences(device.get_handle(), 1, &inFlightFences[currentFrame]);
    VK_CHECK_RESULT(
        vkQueueSubmit(graphic_queue->get_handle(), 1, &submitInfo, inFlightFences[currentFrame]));

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores    = signalSemaphores;
    VkSwapchainKHR swapChains[]    = {swapchain->get_handle()};
    presentInfo.swapchainCount     = 1;
    presentInfo.pSwapchains        = swapChains;
    presentInfo.pImageIndices      = &image_index;
    presentInfo.pResults           = nullptr; // Optional

    result = present_queue->present(presentInfo);
    // present_queue->wait_idle();

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    {
        update_swapchain();
    }

    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

    render_time = std::chrono::duration<float, std::chrono::milliseconds::period>(
                      std::chrono::high_resolution_clock::now() - render_time_marker)
                      .count();
    frame_time = std::chrono::duration<float, std::chrono::milliseconds::period>(
                     std::chrono::high_resolution_clock::now() - frame_time_marker)
                     .count();
    frame_time_marker = std::chrono::high_resolution_clock::now();
}

LogicalDevice &RenderSession::get_device() const
{
    return device;
}

Instance &RenderSession::getInstance() const
{
    return instance;
}

Swapchain &RenderSession::get_swapchain() const
{
    return *swapchain;
}

Window &RenderSession::get_window() const
{
    return *window;
}

VkSurfaceKHR &RenderSession::get_surface()
{
    return surface;
}

RenderPass &RenderSession::get_render_pass()
{
    return *render_pass;
}

void RenderSession::createSemaphores()
{
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(swapchain->get_images().size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        if (vkCreateSemaphore(device.get_handle(), &semaphoreInfo, nullptr,
                              &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device.get_handle(), &semaphoreInfo, nullptr,
                              &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device.get_handle(), &fenceInfo, nullptr, &inFlightFences[i]) !=
                VK_SUCCESS)
            throw std::runtime_error("createSemaphores: failed to create semaphores!");
    }
}

void RenderSession::destroySemaphores()
{
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
    {
        vkDestroySemaphore(device.get_handle(), renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(device.get_handle(), imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(device.get_handle(), inFlightFences[i], nullptr);
    }

    inFlightFences.clear();
    imagesInFlight.clear();

    imageAvailableSemaphores.clear();
    renderFinishedSemaphores.clear();
}

void RenderSession::load_scene(std::shared_ptr<cruelScene::Scene> new_scene)
{
    if (scene != nullptr)
        scene.reset();
    scene = new_scene;
    for (auto &a : scene->get_objs())
    {
        switch (a->get_type())
        {
            case cruelScene::ObjectType::SCENE_OBJ_TYPE_GEOM:
                tasks.push_back(std::make_unique<GeomTask>(*this, a));
                break;
            default:
                break;
        }
    }
}

bool RenderSession::is_session_alive()
{
    if (!glfwWindowShouldClose(&window->get_handle()))
    {
        // vkDeviceWaitIdle(device.get_handle());
        return true;
    }
    return false;
}

void RenderSession::update_swapchain()
{
    int width = 0, height = 0;
    glfwGetFramebufferSize(&window->get_handle(), &width, &height);
    while (width == 0 || height == 0)
    {
        glfwGetFramebufferSize(&window->get_handle(), &width, &height);
        glfwWaitEvents();
    }
    vkDeviceWaitIdle(device.get_handle());

    destroySemaphores();
    frameBuffer.clear();
    render_pass.reset();
    swapchain = std::make_unique<Swapchain>(*swapchain, VkExtent2D({(u32) width, (u32) height}));

    prepare_render_pass();

    for (auto &image_view : swapchain->get_imageViews())
    {
        frameBuffer.push_back(std::make_unique<FrameBuffer>(
            device, image_view, swapchain->get_properties().extent, *render_pass));
    }

    createSemaphores();

    commandBuffers.clear();
    commandPool->ResetPool();

    for (size_t i = 0; i < imgCount; i++)
    {
        commandBuffers.push_back(
            commandPool->RequestCommandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY));
    }

#ifdef RENDER_DEBUG
    commandPool->test_list_commands();
#endif

    if (guiOverlay != nullptr)
    {
        guiOverlay->resize(swapchain->get_properties().extent.width,
                           swapchain->get_properties().extent.height);
        if (guiOverlay->needUpdate())
        {
            guiOverlay->update();
#ifdef RENDER_DEBUG
            std::cout << "[Session::render_frame] guiOverlay needs Update." << std::endl;
#endif
        }
#ifdef RENDER_DEBUG
        else
            std::cout << "[Session::render_frame] guiOverlay doesn't need Update." << std::endl;
#endif
    }

    for (auto &task : tasks)
    {
        task->update_render_pass(*render_pass);
    }

    draw();
}

CommandPool &RenderSession::getCommandPool()
{
    return *commandPool;
}

void RenderSession::set_session_id(u32 new_id)
{
    session_id = new_id;
}

u32 RenderSession::get_session_id() const
{
    return session_id;
}

Queue *RenderSession::get_graphic_queue() const
{
    return graphic_queue;
}

Queue *RenderSession::get_present_queue() const
{
    return present_queue;
}

float RenderSession::getRenderTime() const
{
    return render_time;
}

float RenderSession::getFrameTime() const
{
    return frame_time;
}
} // namespace cruelEngine::cruelRender