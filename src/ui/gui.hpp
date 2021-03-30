#pragma once

#include "../common.h"
#include "../render/render_header.h"
#include "../types.h"

#include "../../external/imgui/imgui.h"

namespace cruelEngine::cruelGui
{
class Gui : public cruelRender::GuiOverlay
{
public:
    Gui(cruelRender::RenderSession &session);

    ~Gui();

    bool needUpdate() const
    {
        return updated;
    }

    void Draw(cruelRender::CommandBuffer &commandBuffer);

    void updateBuffer();

    const float getScale() const
    {
        return scale;
    }

    void newFrame();

    cruelRender::RenderSession &getSession();

    void resize(uint32_t width, uint32_t height);

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
    // std::shared_ptr<cruelRender::CommandBuffer>       singleTimeCmd{};

    std::shared_ptr<cruelRender::Image>     fontImage{};
    std::shared_ptr<cruelRender::ImageView> fontView{};
    std::shared_ptr<cruelRender::Sampler>   sampler{};

#ifdef guiVertexblockSize
#    undef guiVertexblockSize
#endif
#define guiVertexblockSize 500

#ifdef guiVertexmaxSize
#    undef guiVertexmaxSize
#endif
#define guiVertexmaxSize 100 * guiVertexblockSize

    std::shared_ptr<cruelRender::Buffer> vertexBuffer{};

#ifdef guiIndexblockSize
#    undef guiIndexblockSize
#endif
#define guiIndexblockSize 500

#ifdef guiIndexmaxSize
#    undef guiIndexmaxSize
#endif
#define guiIndexmaxSize 100 * guiIndexblockSize
    std::shared_ptr<cruelRender::Buffer> indexBuffer{};

    struct PushConstBlock
    {
        glm::vec2 scale;
        glm::vec2 translate;
    } pushConstBlock;

    float scale         = 1;
    bool  visible       = true;
    bool  bufferUpdated = false;
    bool  updated       = false;
};
} // namespace cruelEngine::cruelGui