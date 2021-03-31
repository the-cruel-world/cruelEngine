#pragma once

#include "../common.h"
#include "../render/render_header.h"
#include "../types.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

#include "TextEditor.h"
#include "imgui_markdown.h"
#include "implot.h"

// #ifdef ImDrawIdx
// #    undef ImDrawIdx
// #    define ImDrawIdx unsigned int
// #endif

// #ifndef ImDrawIdx
// #    define ImDrawIdx unsigned int
// #endif

namespace cruelEngine::cruelGui
{
class Gui : public cruelRender::GuiOverlay
{
public:
    enum GuiUsageFlagBits
    {
        GUI_ONLY_IMGUI      = 0x00000001,
        GUI_ENABLE_IMPLOT   = 0x00000002,
        GUI_ENABLE_MARKDOWN = 0x00000004,
        GUI_ENABLE_DOCKING  = 0x00000008,
    };
    typedef u32 GuiUsageFlags;

    Gui(cruelRender::RenderSession &session, GuiUsageFlags usage);

    Gui(cruelRender::RenderSession &session);

    ~Gui();

    bool needUpdate() const
    {
        return updated;
    }

    void requireUpdate()
    {
        updated = true;
    }

    void Draw(cruelRender::CommandBuffer &commandBuffer);

    void update();

    const float getScale() const
    {
        return scale;
    }

    const GuiUsageFlags getGuiFlags() const
    {
        return flags;
    }

    void renderFrame();

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

    float scale         = 1.0;
    bool  visible       = true;
    bool  bufferUpdated = false;
    bool  updated       = false;

    GuiUsageFlags flags{GuiUsageFlagBits::GUI_ONLY_IMGUI};
};
} // namespace cruelEngine::cruelGui