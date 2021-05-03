#pragma once
#include "vkcommon.h"
/** \bug
 * cant change the order of vkcommon and window/window it's so weird.
 */
#include "../window/window.hpp"

namespace cruelEngine
{
class RenderContext;
class Window;
struct WindowProp;

namespace cruelScene
{
class Scene;
}

namespace cruelRender
{
class LogicalDevice;
class PhysicalDevice;
class CommandBuffer;
class CommandPool;
class Swapchain;
class RenderPass;
class FrameBuffer;
class Instance;
class RenderTask;
class Queue;
class GuiOverlay;
class RenderTarget;
class RenderFrame;
class SubPass;
class RenderPipeline;
class ResourceCache;

typedef enum RendererOptionBits
{
    RENDER_OPTION_DEFAULT_BIT                           = 0,
    RENDER_OPTION_SHADOW_ENABLE_BIT                     = 1 << 0,
    RENDER_OPTION_SHADOW_CASCADE_ENABLE_BIT             = 1 << 1,
    RENDER_OPTION_FOG_ENABLE_BIT                        = 1 << 2,
    RENDER_OPTION_ENVIRONMENT_ENABLE_BIT                = 1 << 3,
    RENDER_OPTION_REFRACTION_ENABLE_BIT                 = 1 << 4,
    RENDER_OPTION_POSITIONAL_LIGHT_ENABLE_BIT           = 1 << 5,
    RENDER_OPTION_POSITIONAL_LIGHT_SHADOW_ENABLE_BIT    = 1 << 6,
    RENDER_OPTION_POSITIONAL_LIGHT_CLUSTER_LIST_BIT     = 1 << 7,
    RENDER_OPTION_SHADOW_VSM_BIT                        = 1 << 8,
    RENDER_OPTION_POSITIONAL_LIGHT_SHADOW_VSM_BIT       = 1 << 9,
    RENDER_OPTION_SHADOW_PCF_KERNEL_WIDTH_3_BIT         = 1 << 10,
    RENDER_OPTION_SHADOW_PCF_KERNEL_WIDTH_5_BIT         = 1 << 11,
    RENDER_OPTION_VOLUMETRIC_FOG_ENABLE_BIT             = 1 << 12,
    RENDER_OPTION_ALPHA_TEST_DISABLE_BIT                = 1 << 13,
    RENDER_OPTION_POSITIONAL_LIGHT_CLUSTER_BINDLESS_BIT = 1 << 14,
    RENDER_OPTION_MULTIVIEW_BIT                         = 1 << 15,
    RENDER_OPTION_AMBIENT_OCCLUSION_BIT                 = 1 << 16
} RendererOptionBits;
typedef u32 RendererOptionFlags;

struct SessionProp
{
    WindowProp          windowProp;
    RendererOptionFlags render_features  = RENDER_OPTION_DEFAULT_BIT;
    float               frame_rate_limit = 100;
    bool                vsync            = false;
};

/**
 * \brief RenderSession renders one scene.
 *
 * The RenderSession creates and maintains windows. It also creates surface, guioverlay, swapchain,
 * renderpass and rendertasks. If the gui overlay is enabled, the render of tasks should be
 * seperated form the gui render process. This is for the better performance.
 *
 * \todo
 * 1. RenderSession should support headless render.
 * 2. Sessions should support multi swapchains/renderpass.
 * 3. Sessions should run seperated thread.
 */
class RenderSession
{
public:
    /**
     * \brief RenderSession Constructor.
     *
     * @param instance is needed to create vulkan khr surface.
     * @param device is required to process the rendering task.
     * @param session_properties is the settings for a RenderSession.
     */
    RenderSession(Instance &instance, LogicalDevice &device, SessionProp &session_properties);

    RenderSession(const RenderSession &) = delete;

    RenderSession &operator=(const RenderSession &) = delete;

    ~RenderSession();

    /**
     * \brief update the swapchain
     * When the a window_resize event is captured, execute this function
     * to rebuild a new swapchain. Okay, it should be the resize function.
     */
    void update_swapchain();

    void recreate_swapchain()
    {}

    /**
     * \brief LoadScene
     * Load a scene for rendering, The load process will also create rendertasks for all primitives
     * in the scene.
     */
    void load_scene(std::shared_ptr<cruelScene::Scene> scene);

    /**
     * \brief AddTasks
     * Add new tasks to the RenderSession. This is for external tasks.
     */
    void add_new_task(std::unique_ptr<RenderTask> task);

    /**
     * \brief Prepare
     * Run the preparation work for renderSession.
     */
    void prepare();

    /**
     * \brief Create render_pass
     * Create a proper render pass according to the render properties.
     * Use subpasses. see [render pass best practice];
     */
    void prepare_render_pipeline();

    /**
     * \brief Draw
     * Draw all render tasks, gui included.
     */
    void draw();

    void BeginFrame();

    void EndFrame();

    void Submit();

    RenderFrame &GetAcriveFrame();

    u32 GetActiveFrameIdx();

    /**
     * \brief CreateGuiStaff
     * CreateGuiStaff creates swapchain, renderpass.
     */
    void CreateGuiStaff();

    /**
     * \brief CreateSceneStaff
     * CreateSceneStaff creates render tasks for scene.
     */
    void CreateSceneStaff();

    /**
     * \brief DrawGui
     * DrawGui draws gui. Records all draw commands related to the gui.
     */
    void DrawGui();

    /**
     * \brief DrawScene
     * DrawScene records all draw commands related to the scene. Infact, it just call all
     * renderTasks' draw command.
     */
    void DrawScene();

    /**
     * \brief RenderGui
     * RenderGui submit the draw commands of gui to graphic queue.
     */
    void RenderGui();

    /**
     * \brief RenderScene
     * RenderGui submit the draw commands of gui to graphic queue.
     */
    void RenderScene();

    /**
     * \brief RenderTarget
     * Draw a new frame.
     */
    void render_frame();

    LogicalDevice &get_device() const;
    Instance &     getInstance() const;
    Swapchain &    get_swapchain() const;
    VkFormat       GetFormat();
    Window &       get_window() const;

    VkSurfaceKHR &get_surface();
    VkExtent2D    GetExtent();
    CommandPool & getCommandPool();

    void set_session_id(u32 new_id);
    u32  get_session_id() const;

    Queue *get_graphic_queue() const;
    Queue *get_present_queue() const;

    float getRenderTime() const;
    float getFrameTime() const;

    const std::vector<std::unique_ptr<RenderTask>> &GetTasks() const;

    /**
     * \brief SetGuiOverlay
     * Set the guiOverlay for the RenderSession. If the pointer to guiOverlay is "null_ptr", the
     * session will not render the guioverlay.
     */
    void setGuiOverlay(std::shared_ptr<GuiOverlay> gui);

    std::shared_ptr<GuiOverlay> getGuiOverlay();

    bool is_session_alive();
    void createSemaphores();
    void destroySemaphores();

private:
    float render_time = 0.0f;
    float frame_time  = 0.0f;

    float frame_rate_limit = 100.0f;

    std::chrono::time_point<std::chrono::high_resolution_clock> render_time_marker{};
    std::chrono::time_point<std::chrono::high_resolution_clock> frame_time_marker{};

    u32 session_id = 0;
    u32 imgCount   = 3;

    VkSemaphore imageAvailableSemaphore = VK_NULL_HANDLE;
    VkSemaphore renderFinishedSemaphore = VK_NULL_HANDLE;
    VkFence     inFlightFences          = VK_NULL_HANDLE;
    // u32         image_index             = 0;

    //! \brief The reference points to the instance of the render context
    Instance &instance;

    //! \brief The reference points to the logical device of the render context.
    LogicalDevice &device;

    //! \brief The unique window of a render session, it's null if the session is
    //! a headless session.
    std::unique_ptr<Window> window;

    //! \brief The surface of a session. Every session will have one unless it's a
    //! headless render session.
    VkSurfaceKHR surface = VK_NULL_HANDLE;

    //! \brief The swapchain of this session. Every session should have at least
    //! one swapchain.
    std::unique_ptr<Swapchain> swapchain;

    //! \brief The redner properties of a render session.
    SessionProp session_properties;

    std::shared_ptr<GuiOverlay> guiOverlay{};

    /**
     * The callback function react to window_resize_event.
     */
    void (*on_window_resize_callback)(GLFWwindow *window, int width, int height) = nullptr;

    Queue *graphic_queue;
    Queue *present_queue;

    std::shared_ptr<cruelScene::Scene> scene;

    std::unique_ptr<CommandPool> commandPool;

    std::vector<std::unique_ptr<RenderFrame>> RenderFrame;

    std::unique_ptr<RenderPipeline> renderPipeline;

    //    std::vector<std::unique_ptr<RenderPipeline>> renderPipelines{};

    std::vector<std::unique_ptr<RenderTask>> tasks;
};

void on_window_resize_cb(GLFWwindow *window, int width, int height);
} // namespace cruelRender
} // namespace cruelEngine