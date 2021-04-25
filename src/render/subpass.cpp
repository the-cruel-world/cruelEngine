//
// Created by yiwen on 4/20/21.
//

#include "subpass.hpp"

namespace cruelEngine::cruelRender
{
SubPass::SubPass(RenderSession &session) : session{session}
{}

RenderSession &SubPass::GetSession()
{
    return session;
}
}; // namespace cruelEngine::cruelRender