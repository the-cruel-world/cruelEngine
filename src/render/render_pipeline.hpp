//
// Created by yiwen on 4/25/21.
//

#ifndef CRUELENGINE_RENDERPIPELINE_HPP
#define CRUELENGINE_RENDERPIPELINE_HPP

#pragma once
#include "vkcommon.h"

namespace cruelEngine::cruelRender
{
class LogicalDevice;
class RenderPass;
class SubPass;
class RenderTask;
class RenderTarget;

/**
 * RenderPipeline is the pipeline for my render api.
 * It's just the workflow of this engine, don't mix it with the render pipeline.
 *
 * RenderPipeline contains subpasses. Which defines the render precess.
 * More subpasses can be added to the sequence if required.
 * For example, postprocessing can be implemented with two pipelines which
 * share render targets.
 * */

class RenderPipeline
{
public:
    /**
     * RenderPipeline is the pipeline for my render api.
     *
     * @param SubPass The render passes required for this pipeline.
     * */
    RenderPipeline(LogicalDevice &device, std::vector<std::unique_ptr<SubPass>> &&SubPass = {});

    RenderPipeline(const RenderPipeline &) = delete;

    //    RenderPipeline(RenderPipeline &&) = default;

    RenderPipeline &operator=(const RenderPipeline &) = delete;

    //    RenderPipeline &operator=(RenderPipeline &&) = default;

    virtual ~RenderPipeline() = default;

    void Draw(RenderTarget &target);

    void Prepare();

    std::unique_ptr<SubPass> &GetActiveSubpass();

    void AddSubpass(std::unique_ptr<SubPass> &&subpass);

    std::vector<std::unique_ptr<SubPass>> &GetSubPasses();

    RenderPass &GetRenderPass();

    void UpdateTasks();

    const std::vector<std::unique_ptr<RenderTask>> &GetTasks() const;

    void BuildRenderPass();

private:
    LogicalDevice &device;
    //! \brief The renderpass of this pipeline. Every session should have at least
    //! one render pass.
    std::unique_ptr<RenderPass> renderPass;

    std::vector<std::unique_ptr<SubPass>> subpasses;

    std::vector<std::unique_ptr<RenderTask>> tasks;

    u32 active_subpass_idx{0};

    u32 active_task_idx{0};
};

} // namespace cruelEngine::cruelRender
#endif // CRUELENGINE_RENDERPIPELINE_HPP