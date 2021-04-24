//
// Created by yiwen on 4/20/21.
//

#ifndef CRUELENGINE_SUBPASS_HPP
#define CRUELENGINE_SUBPASS_HPP
#pragma once
#include "vkcommon.h"

#include "render/vulkan/render_pass.hpp"

namespace cruelEngine::cruelScene
{
class Scene;
}

namespace cruelEngine::cruelRender
{
class RenderSession;
class CommandBuffer;

class SubPass
{
public:
    /**
     * Base class for multi-pass rendering.
     *
     * @param session the session that the renderpass will be executed in.
     * */
    SubPass(RenderSession &session);

    SubPass(const SubPass &) = delete;

    SubPass(SubPass &&) = default;

    SubPass &operator=(const SubPass &) = delete;

    SubPass &operator=(SubPass &&) = delete;

    virtual ~SubPass() = default;

    virtual void draw(CommandBuffer &commandBuffer) = 0;

    virtual void prepare() = 0;

    RenderSession &GetSession();

    // The configurations of a subpass, these are from the kronosGroup's framework. I need to change
    // them to a simpler form
    std::vector<uint32_t> get_input_attachments();

    std::vector<uint32_t> get_output_attachments();

    std::vector<uint32_t> get_color_resolve_attachments();

    bool get_disable_depth_stencil_attachment();

    uint32_t get_depth_stencil_resolve_attachment();

    VkResolveModeFlagBits get_depth_stencil_resolve_mode();

protected:
    RenderSession &session;

    std::unique_ptr<RenderPass> renderPass{nullptr};

    std::vector<uint32_t> input_attachments{};

    std::vector<uint32_t> output_attachments{};

    std::vector<uint32_t> color_resolve_attachments{};

    bool disable_depth_stencil_attachment{false};

    uint32_t depth_stencil_resolve_attachment{VK_ATTACHMENT_UNUSED};

    VkResolveModeFlagBits depth_stencil_resolve_mode{VK_RESOLVE_MODE_NONE};
};
}; // namespace cruelEngine::cruelRender

#endif // CRUELENGINE_SUBPASS_HPP
