#pragma once
#include "vkcommon.h"

#include "vkDevice.hpp"
#include "swapChain.hpp"
#include "pipeLine.hpp"
#include "renderPass.hpp"
#include "frameBuffer.hpp"
#include "commandPool.hpp"

namespace cruelEngine {
namespace VulkanContext {
    // struct CommandBuffer {
    // public:
    //     CommandBuffer(const VulkanDevice &_device, const SwapChain &_swapChain,
    //         const RenderPass &_renderPass,  
    //         const PipeLine &_pipeLine, const FrameBuffer &_frameBuffer)
    //         : device(_device), swapChain(_swapChain), renderPass(_renderPass),
    //         pipeLine(_pipeLine), frameBuffers(_frameBuffer)
    //     {

    //     }
    //     virtual ~CommandBuffer();

    //     void                        commandBegine();
    //     void                        commandEnd();
        
    //     void                        createCommandBuffer();
    //     void                        destroyCommandBuffer();
    //     void                        createCommandPool();
    //     void                        destroyCommandPool();
    // public: 
    //     std::vector<VkCommandBuffer> commandBuffers = {};
    //     VkCommandPool commandPool = VK_NULL_HANDLE;

    // private:
    //     const VulkanDevice          &device;
    //     const SwapChain             &swapChain;
    //     const RenderPass            &renderPass;
    //     const PipeLine              &pipeLine;
    //     const FrameBuffer           &frameBuffers;
    // };
    /*! A simple wrap of Vulkan commandBuffer for easy managenent. */
    struct CommandBuffer {
    public:
        /*! \brief Init the command buffer, allocate space for it. */
        CommandBuffer(const CommandPool &_commandPool, VkCommandBufferLevel _level);
        virtual ~CommandBuffer();

        /*! \brief Return a reference to the Vulkan CommandBuffer. */ 
        const VkCommandBuffer&      get_handle() const {return handle;}

        /*! \brief Whether this command buffer is in recoording state. Return true if yes. */ 
        bool                        isRecording();

        /*! \brief Start to record. When the recording is finished, remember to call end(). */
        void                        begin();

        /*! \brief End the process of record, then the command is ready to use. */
        void                        end();

        void                        begin_renderpass(const VkRenderPass &renderPass, const VkFramebuffer &frameBuffer, const VkExtent2D extent2dMode);

        void                        end_renderpass();

        /*! \brief Execute this recorded commandbuffer immidiately. */
        void                        exec();

        void                        bindPipeLine(VkPipeline& pipeLine, VkPipelineBindPoint flag);
        void                        setViewPort(){}
        void                        setScissor(){}
        
    public: 
        

    private:

        const CommandPool           &commandPool;

        //RenderPass                  &renderPass;

        VkCommandBuffer             handle = VK_NULL_HANDLE;
        
        bool                        isRecord = false;

        VkCommandBufferLevel        level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    };
}
}