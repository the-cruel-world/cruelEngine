#include "render/render_session.hpp"
#include "render/gui_overlay.hpp"
#include "render/render_pipeline.hpp"
#include "render/render_target.hpp"
#include "render/render_task.hpp"
#include "render/subpass.hpp"
#include "render/subpasses/geompass.hpp"

#include "scene/object.hpp"
#include "scene/scene.hpp"

#include "render/render_tasks/render_tasks.hpp"
#include "render/vulkan/command_buffer.hpp"
#include "render/vulkan/command_pool.hpp"
#include "render/vulkan/frame_buffer.hpp"
#include "render/vulkan/instance.hpp"
#include "render/vulkan/logical_device.hpp"
#include "render/vulkan/queue.hpp"
#include "render/vulkan/render_pass.hpp"
#include "render/vulkan/swapchain.hpp"

namespace cruelEngine::cruelRender
{
RenderSession::RenderSession(Instance &instance, LogicalDevice &device,
                             SessionProp &session_properties) :
    instance{instance}, device{device}, session_properties{session_properties}
{
    //! Create a new window for this session.
    window                    = std::make_unique<Window>(session_properties.windowProp);
    on_window_resize_callback = on_window_resize_cb;
    glfwSetFramebufferSizeCallback(&window->get_handle(), on_window_resize_callback);

    //! create a render surface for this session.
    VK_CHECK_RESULT(
        glfwCreateWindowSurface(instance.get_handle(), &(window->get_handle()), nullptr, &surface));
    CRUEL_LOG("%s", "Session window Created\n");

    VkExtent2D extent = {};

    graphic_queue = &device.get_suitable_graphics_queue(0);
    present_queue = &device.get_suitable_present_queue(surface, 1);

    CRUEL_LOG("Session Graphic Queue selected %d:%d\n", graphic_queue->get_family_index(),
              graphic_queue->get_index());

    CRUEL_LOG("Session Present Queue selected %d:%d\n", present_queue->get_family_index(),
              present_queue->get_index());

    /**
     * If vsync if enabled, use mailbox, or use the immediate mode (frame rate unlimited.)*/
    swapchain = std::make_unique<Swapchain>(
        device, surface, extent, imgCount, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,
        session_properties.vsync == true ? VK_PRESENT_MODE_MAILBOX_KHR :
                                           VK_PRESENT_MODE_IMMEDIATE_KHR);

    CRUEL_LOG("%s\n", "Session SwapChain init-ed.");

    /**
     * Methods to create subpasses. according to the session properties. */
    //    prepare_render_pass();

    commandPool = std::make_unique<CommandPool>(device, graphic_queue->get_family_index(),
                                                CommandBuffer::ResetModeFlags::ResetIndividually);

    CRUEL_LOG("%s\n", "Session CommandPool created");

    VkExtent3D         extent3D{swapchain->get_extent().width, swapchain->get_extent().height, 1};
    std::vector<Image> images;
    for (auto &image : swapchain->get_images())
    {
        Image swapchain_image(device, image, extent3D, swapchain->get_surface_format().format,
                              swapchain->get_properties().image_usage);
        images.push_back(std::move(swapchain_image));
        //        ImageView swapchain_imageView(device, &swapchain_image,
        //        swapchain_image.get_format());
    }

    renderTargets.push_back(std::make_unique<RenderTarget>(device, std::move(images)));

    CRUEL_LOG("%s\n", "Session RenderTargets created");

#ifdef RENDER_DEBUG
    commandPool->test_list_commands();
#endif

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    VK_CHECK_RESULT(
        vkCreateSemaphore(device.get_handle(), &semaphoreInfo, nullptr, &imageAvailableSemaphore));
    VK_CHECK_RESULT(
        vkCreateSemaphore(device.get_handle(), &semaphoreInfo, nullptr, &renderFinishedSemaphore));

    CRUEL_LOG("%s\n", "Session Semaphores created");

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
    graphic_queue->wait_idle();
    present_queue->wait_idle();

    if (imageAvailableSemaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(device.get_handle(), imageAvailableSemaphore, nullptr);
    }
    if (renderFinishedSemaphore != VK_NULL_HANDLE)
    {
        vkDestroySemaphore(device.get_handle(), renderFinishedSemaphore, nullptr);
    }

    //    renderFrames.clear();
    swapchain.reset();

    tasks.clear();
    guiOverlay.reset();

    commandPool->ResetPool();
    commandPool.reset();

    if (surface != VK_NULL_HANDLE)
    {
        vkDestroySurfaceKHR(instance.get_handle(), surface, nullptr);
        surface = VK_NULL_HANDLE;
    }
    window.reset();

    CRUEL_LOG("Session: %p destroyed\n", this);
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

void RenderSession::draw()
{
#ifdef RENDER_DEBUG
    std::cout << "Session tasks: " << tasks.size() << std::endl;
#endif
    if (tasks.size() == 0 && guiOverlay == nullptr)
        return;

    graphic_queue->wait_idle();

    //    for (auto &renderFrame : renderFrames)
    //    {
    //        //        renderFrame->RecordBegin();
    //        VkViewport viewport{};
    //        viewport.x        = 0.0f;
    //        viewport.y        = 0.0f;
    //        viewport.width    = static_cast<float>(swapchain.get()->get_extent().width);
    //        viewport.height   = static_cast<float>(swapchain.get()->get_extent().height);
    //        viewport.minDepth = 0.0f;
    //        viewport.maxDepth = 1.0f;
    //        //        renderFrame->GetCommandBuffer().setViewport(0, {viewport});
    //        VkRect2D scissor{};
    //        scissor.offset = {0, 0};
    //        scissor.extent = swapchain.get()->get_extent();
    //        //        renderFrame->GetCommandBuffer().setScissor(0, {scissor});
    //
    //        //        for (auto &task : tasks)
    //        //        {
    //        //            task->draw(renderFrame->GetCommandBuffer(), 0);
    //        //        }
    //
    //        //        if (guiOverlay != nullptr)
    //        //        {
    //        //            vkCmdSetViewport(renderFrame->GetCommandBuffer().get_handle(), 0, 1,
    //        //            &viewport);
    //        vkCmdSetScissor(renderFrame->GetCommandBuffer().get_handle(), 0,
    //        //            1, &scissor); guiOverlay->Draw(renderFrame->GetCommandBuffer());
    //        //        }
    //
    //        //        renderFrame->RecordEnd();
    //    }
}

void RenderSession::render_frame()
{
    //    render_time_marker = std::chrono::high_resolution_clock::now();
    //
    //    RENDER_LOG("frames in flight: %d frames\n",
    //               static_cast<int>(std::count_if(
    //                   renderFrames.begin(), renderFrames.end(),
    //                   [](std::unique_ptr<RenderTarget> const &frame) { return frame->GetStatus();
    //                   })));
    //
    //    u32      image_index;
    //    VkResult result = swapchain->acquire_next_image(image_index, imageAvailableSemaphore);
    //
    //    vkWaitForFences(device.get_handle(), 1, &renderFrames[image_index]->GetFence(), VK_TRUE,
    //                    UINT64_MAX);
    //    renderFrames[image_index]->SetStatus(false);
    //
    //    VkSemaphore tempSemaphore                     =
    //    renderFrames[image_index]->GetImageAvaiable();
    //    renderFrames[image_index]->GetImageAvaiable() = imageAvailableSemaphore;
    //    imageAvailableSemaphore                       = tempSemaphore;
    //
    //    if (result == VK_ERROR_OUT_OF_DATE_KHR)
    //    {
    //        update_swapchain();
    //        return;
    //    }
    //
    //    if (guiOverlay != nullptr)
    //    {
    //        guiOverlay->renderFrame();
    //        if (guiOverlay->needUpdate())
    //        {
    //            guiOverlay->update();
    //#ifdef RENDER_DEBUG
    //            std::cout << "[Session::render_frame] guiOverlay needs Update." << std::endl;
    //#endif
    //            draw();
    //        }
    //#ifdef RENDER_DEBUG
    //        else
    //            std::cout << "[Session::render_frame] guiOverlay doesn't need Update." <<
    //            std::endl;
    //#endif
    //    }
    //
    //    for (auto &task : tasks)
    //    {
    //        task->render();
    //    }
    //
    //    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    //
    //    VkSemaphore waitSemaphores[]   = {renderFrames[image_index]->GetImageAvaiable()};
    //    VkSemaphore signalSemaphores[] = {renderFrames[image_index]->GetRenderFinished()};
    //
    //    VkSubmitInfo submitInfo{VK_STRUCTURE_TYPE_SUBMIT_INFO};
    //    submitInfo.commandBufferCount = 1;
    //    //    submitInfo.pCommandBuffers      =
    //    //    &(renderFrames[image_index]->GetCommandBuffer().get_handle());
    //    submitInfo.waitSemaphoreCount   = 1;
    //    submitInfo.pWaitSemaphores      = waitSemaphores;
    //    submitInfo.pWaitDstStageMask    = waitStages;
    //    submitInfo.signalSemaphoreCount = 1;
    //    submitInfo.pSignalSemaphores    = signalSemaphores;
    //
    //    vkWaitForFences(device.get_handle(), 1, &renderFrames[image_index]->GetFence(), VK_TRUE,
    //                    UINT64_MAX);
    //    vkResetFences(device.get_handle(), 1, &renderFrames[image_index]->GetFence());
    //    VK_CHECK_RESULT(vkQueueSubmit(graphic_queue->get_handle(), 1, &submitInfo,
    //                                  renderFrames[image_index]->GetFence()));
    //    renderFrames[image_index]->SetStatus(true);
    //
    //    VkSwapchainKHR swapChains[] = {swapchain->get_handle()};
    //
    //    VkPresentInfoKHR presentInfo{};
    //    presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    //    presentInfo.waitSemaphoreCount = 1;
    //    presentInfo.pWaitSemaphores    = signalSemaphores;
    //    presentInfo.swapchainCount     = 1;
    //    presentInfo.pSwapchains        = swapChains;
    //    presentInfo.pImageIndices      = &image_index;
    //    presentInfo.pResults           = nullptr; // Optional
    //
    //    result = present_queue->present(presentInfo);
    //
    //    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
    //    {
    //        update_swapchain();
    //    }

    render_time = std::chrono::duration<float, std::chrono::milliseconds::period>(
                      std::chrono::high_resolution_clock::now() - render_time_marker)
                      .count();
    frame_time = std::chrono::duration<float, std::chrono::milliseconds::period>(
                     std::chrono::high_resolution_clock::now() - frame_time_marker)
                     .count();
    frame_time_marker = std::chrono::high_resolution_clock::now();

    RENDER_LOG("frame time: %.3fms\trendering time: %.3fms\tframe rate: %.3ffps\n",
               static_cast<float>(frame_time), static_cast<float>(render_time),
               static_cast<float>(1e3 / frame_time));
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

/** \todo Complete this part after scene graph.*/
void RenderSession::load_scene(std::shared_ptr<cruelScene::Scene> new_scene)
{
    if (scene != nullptr)
        scene.reset();
    scene = new_scene;
    // for (auto &a : scene->get_objs())
    // {
    //     switch (a->get_type())
    //     {
    //         case cruelScene::ObjectType::SCENE_OBJ_TYPE_GEOM:
    //             tasks.push_back(std::make_unique<GeomTask>(*this, a));
    //             break;
    //         default:
    //             break;
    //     }
    // }
}

bool RenderSession::is_session_alive()
{
    if (!glfwWindowShouldClose(&window->get_handle()))
    {
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

    graphic_queue->wait_idle();
    present_queue->wait_idle();

    //    renderFrames.clear();
    swapchain = std::make_unique<Swapchain>(*swapchain, VkExtent2D({(u32) width, (u32) height}));

    //    prepare_render_pass();

    commandPool->ResetPool();

#ifdef RENDER_DEBUG
    commandPool->test_list_commands();
#endif

    VkExtent3D extent3D{swapchain->get_extent().width, swapchain->get_extent().height, 1};

    for (auto &image : swapchain->get_images())
    {
        Image     swapchain_image(device, image, extent3D, swapchain->get_surface_format().format,
                              swapchain->get_properties().image_usage);
        ImageView swapchain_imageView(device, &swapchain_image, swapchain_image.get_format());

        //        renderFrames.push_back(
        //            std::make_unique<RenderTarget>(device, (swapchain_image),
        //            (swapchain_imageView)));
    }

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
const std::vector<std::unique_ptr<RenderTask>> &RenderSession::GetTasks() const
{
    return tasks;
}
} // namespace cruelEngine::cruelRender