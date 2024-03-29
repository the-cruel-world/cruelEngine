#pragma once
#include "../vkcommon.h"

namespace cruelEngine::cruelRender
{
class CommandPool;
class Queue;

/** The data prototype of the commandBuffer. It will include all different types
 * of vulkan command. */
class CommandBuffer
{
public:
    enum ResetModeFlags
    {
        ResetPool,
        ResetIndividually,
        ReAllocate,
    };

    enum CmdStateFlags
    {
        CMD_STATE_FREE      = 0,
        CMD_STATE_RESETED   = 0,
        CMD_STATE_IDLE      = 1,
        CMD_STATE_RECORDING = 2,
        CMD_STATE_BUSY      = 3,
    };

    /** \brief Init the command buffer, allocate space for it.
     * \param _commandPool is the commandPool that this commandbuffer belongs to
     * \param _level is the level of the commandBuffer, can be
     * VK_COMMAND_BUFFER_LEVEL_PRIMARY or VK_COMMAND_BUFFER_LEVEL_SECONDARY
     */
    CommandBuffer(const CommandPool &_commandPool, VkCommandBufferLevel _level);

    CommandBuffer(const CommandBuffer &) = delete;

    CommandBuffer(CommandBuffer &&);

    CommandBuffer &operator=(const CommandBuffer &) = delete;

    CommandBuffer &operator=(CommandBuffer &&) = delete;

    virtual ~CommandBuffer();

    /** \brief Return a reference to the Vulkan CommandBuffer. */
    const VkCommandBuffer &get_handle() const
    {
        return handle;
    }

    /** \brief Whether this command buffer is in recoording state. Return true if
     * yes. */
    bool isRecording();

    /** \brief Start to record. When the recording is finished, remember to call
     * end(). */
    void begin();

    /** \brief End the process of record, then the command is ready to use. */
    void end();

    /** \brief Srart to record a one time command. */
    void beginOneTime();

    /** \brief End the record of one time command. */
    void endOneTime();

    /** \brief Start to record the behavior of the renderpass. */
    void begin_renderpass(const VkRenderPass &renderPass, const VkFramebuffer &frameBuffer,
                          const VkExtent2D extent2dMode);

    void end_renderpass();

    /** \brief Execute this recorded commandbuffer immidiately. */
    void exec(const VkQueue &queue, const VkSemaphore &semaphore);

    /** \brief Bind a pipeLine to this commandBuffer. */
    void bindPipeLine(VkPipeline &pipeLine, VkPipelineBindPoint flag);

    void setViewport(uint32_t first_viewport, const std::vector<VkViewport> &viewports);

    void setScissor(uint32_t first_scissor, const std::vector<VkRect2D> &scissors);

    VkResult reset(ResetModeFlags resetMode);

    void Release();

    void SetOccupied();

    CmdStateFlags GetCmdState();

private:
    const CommandPool &commandPool;

    // RenderPass                  &renderPass;

    VkCommandBuffer handle = VK_NULL_HANDLE;

    CmdStateFlags cmdState = CmdStateFlags::CMD_STATE_IDLE;

    VkCommandBufferLevel level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
};

} // namespace cruelEngine::cruelRender