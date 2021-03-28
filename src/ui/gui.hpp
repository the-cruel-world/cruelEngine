#pragma once

#include "../../external/imgui/imgui.h"
#include "../common.h"
#include "../render/render_header.h"
#include "../types.h"

namespace cruelEngine::cruelGui
{
class Gui : public cruelRender::GuiOverlay
{
public:
    Gui(cruelRender::RenderSession &session);

    ~Gui(){};

    void Draw(cruelRender::CommandBuffer &commandBuffer);

    void newFrame();

    void prepare_resource();
    void prepare_pipeline();
    void free_resource();
    void destroy_pipeline();

    bool header(const char *caption);
    bool checkBox(const char *caption, bool *value);
    bool checkBox(const char *caption, int32_t *value);
    bool inputFloat(const char *caption, float *value, float step, uint32_t precision);
    bool sliderFloat(const char *caption, float *value, float min, float max);
    bool sliderInt(const char *caption, int32_t *value, int32_t min, int32_t max);
    bool comboBox(const char *caption, int32_t *itemindex, std::vector<std::string> items);
    bool button(const char *caption);
    void text(const char *formatstr, ...);

private:
    std::shared_ptr<cruelRender::DescriptorPool>      descriptorPool{};
    std::shared_ptr<cruelRender::DescriptorSetLayout> descriptorSetLayout{};
    std::shared_ptr<cruelRender::DescriptorSet>       descriptorSet{};
    std::shared_ptr<cruelRender::PipelineLayout>      pipelineLayout{};
    std::shared_ptr<cruelRender::Pipeline>            pipeline{};
    std::shared_ptr<cruelRender::Queue>               queue{};

    std::shared_ptr<cruelRender::Image>     fontImage{};
    std::shared_ptr<cruelRender::ImageView> fontView{};
    std::shared_ptr<cruelRender::Sampler>   sampler{};

    float scale   = 0.5;
    bool  visible = true;
    bool  updated = false;
};
} // namespace cruelEngine::cruelGui