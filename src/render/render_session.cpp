#include "render_session.hpp"
#include "render_task.hpp"

#include "vulkan/logical_device.hpp"
#include "vulkan/instance.hpp"
#include "vulkan/swapchain.hpp"
#include "vulkan/queue.hpp"
#include "vulkan/render_pass.hpp"
#include "vulkan/command_buffer.hpp"
#include "vulkan/frame_buffer.hpp"
#include "../window/window.hpp"

namespace cruelEngine {
namespace cruelRender {

    RenderSession::RenderSession (Instance &instance, 
                                  LogicalDevice &device, 
                                  RenderProp &render_properties) :
        instance{instance},
        device {device},
        render_properties{render_properties}
    {
        //! Create a new window for this session.
        window = std::make_unique<Window>();
        //! create a render surface for this session.
        glfwCreateWindowSurface(instance.get_handle(), &(window->get_handle()), nullptr, &surface);

        VkExtent2D extent = {};
        u32 imgCount = 3;
        
        swapchain = std::make_unique<Swapchain>(device, surface, extent, imgCount, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR, VK_PRESENT_MODE_FIFO_KHR);
        
        graphic_queue = &device.get_suitable_graphics_queue(0);
        
        present_queue = &device.get_suitable_present_queue(surface, 0);
        
        std::cout << "graphic queue info: " << graphic_queue->get_family_index() << graphic_queue->get_index() << std::endl;
        std::cout << "present queue info: " << present_queue->get_family_index() << present_queue->get_index() << std::endl;
        
        prepare_render_pass();

        for (auto &image_view : swapchain->get_imageViews())
        {
            frameBuffer.push_back(std::make_unique<FrameBuffer>(device, image_view, swapchain->get_properties().extent, *render_pass));
        }

        createSemaphores();

        for (size_t i = 0; i < imgCount; i ++){
            commandBuffers.push_back(device.request_commandBuffer(VK_COMMAND_BUFFER_LEVEL_PRIMARY));
        }
    }

    RenderSession::~RenderSession()
    {
        //! run destructor until all the render process ends.
        vkDeviceWaitIdle(device.get_handle());
        commandBuffers.clear();
        tasks.clear();
        destroySemaphores();
        frameBuffer.clear();
        swapchain.reset();
        vkDestroySurfaceKHR(instance.get_handle(), surface, nullptr);
        window.reset();
        std::cout << "Session destroied" << std::endl;
    }

    RenderTask *RenderSession::request_new_task()
    {
        tasks.push_back(std::make_unique<RenderTask>(*this, *render_pass));
        return tasks.back().get();
    }

    void RenderSession::prepare_render_pass()
    {
        VkAttachmentDescription colorAttachment{};
        colorAttachment.format = swapchain->get_surface_format().format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef{};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass{};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;

        VkSubpassDependency dependency{};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        RenderPassAttachment attachments 
        = {
            .colorAttachment = colorAttachment, 
            .colorAttachmentRef = colorAttachmentRef,
            .subpassDependency = dependency,
            .subpass = {subpass}
        };
        
        render_pass = std::make_unique<RenderPass>(device, attachments);
    }

    void RenderSession::draw()
    {
        size_t size = commandBuffers.size();
        for (size_t i = 0; i < size ; i ++)
        {
            commandBuffers[i].get().begin();
            std::cout << "[session] cmd begin " << i << std::endl;
            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width    = static_cast<float>(swapchain.get()->get_extent().width);
            viewport.height   = static_cast<float>(swapchain.get()->get_extent().height);
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
	        commandBuffers[i].get().setViewport(0, {viewport});
            VkRect2D scissor{};
            scissor.offset = {0, 0};
	        scissor.extent = swapchain.get()->get_extent();
            commandBuffers[i].get().setScissor(0, {scissor});

            commandBuffers[i].get().begin_renderpass(render_pass->get_handle(), frameBuffer[i]->get_handle(), swapchain->get_properties().extent);
            for (auto &task: tasks)
            {
                task->draw(commandBuffers[i].get());
            }
            commandBuffers[i].get().end_renderpass();
            commandBuffers[i].get().end();
            std::cout << "[session] cmd end " << i << std::endl;
        }
    }

    void RenderSession::render_frame()
    {
        u32 image_index;
        VkResult result = swapchain->acquire_next_image(image_index, imageAvailableSemaphores[currentFrame]);

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            std::cout << "vkAcquireNextImageKHR Failed" << std::endl;
        }

        if (imagesInFlight[image_index] != VK_NULL_HANDLE) {
            vkWaitForFences(device.get_handle(), 1, &imagesInFlight[image_index], VK_TRUE, UINT64_MAX);
        }
        imagesInFlight[image_index] = inFlightFences[currentFrame];

        VkSubmitInfo submitInfo{VK_STRUCTURE_TYPE_SUBMIT_INFO};

        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &(commandBuffers[image_index].get().get_handle());

        VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
        VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphores;
        submitInfo.pWaitDstStageMask = waitStages;
        VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        VK_CHECK_RESULT (vkQueueSubmit(graphic_queue->get_handle(), 1, &submitInfo, VK_NULL_HANDLE));

        VkPresentInfoKHR presentInfo{};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;
        VkSwapchainKHR swapChains[] = {swapchain->get_handle()};
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &image_index;
        presentInfo.pResults = nullptr; // Optional

        VK_CHECK_RESULT(present_queue->present(presentInfo));
        present_queue->wait_idle();

        currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
    }

    void RenderSession::createSemaphores() {
        imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
        inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
        imagesInFlight.resize(swapchain->get_images().size(), VK_NULL_HANDLE);

        VkSemaphoreCreateInfo semaphoreInfo{};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo{};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            if (vkCreateSemaphore(device.get_handle(), &semaphoreInfo, nullptr, 
                        &imageAvailableSemaphores[i]) != VK_SUCCESS ||
                    vkCreateSemaphore(device.get_handle(), &semaphoreInfo, nullptr, 
                        &renderFinishedSemaphores[i]) != VK_SUCCESS || 
                    vkCreateFence(device.get_handle(), &fenceInfo, nullptr, 
                        &inFlightFences[i]) != VK_SUCCESS)
                throw std::runtime_error("createSemaphores: failed to create semaphores!");
        }
    }

    void RenderSession::destroySemaphores() {
        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroySemaphore(device.get_handle(), renderFinishedSemaphores[i], nullptr);
            vkDestroySemaphore(device.get_handle(), imageAvailableSemaphores[i], nullptr);
            vkDestroyFence(device.get_handle(), inFlightFences[i], nullptr);
        }
    }

}
}