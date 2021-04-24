//
// Created by yiwen on 4/20/21.
//

#include "subpass.hpp"

namespace cruelEngine::cruelRender
{
SubPass::SubPass(RenderSession &session) : session{session}
{}

SubPass::~SubPass()
{}
RenderSession &SubPass::GetSession()
{
    return session;
}
}; // namespace cruelEngine::cruelRender