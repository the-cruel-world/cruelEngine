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
class RenderFrame;

struct SessionProp
{
    WindowProp window_prop;
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
     *
     * When the a window_resize event is captured, execute this function
     * to rebuild a new swapchain. Okay, it should be the resize function.
     */
    void update_swapchain();

    void recreate_swapchain()
    {}

    /**
     * \brief LoadScene
     *
     * Load a scene for rendering, The load process will also create rendertasks for all primitives
     * in the scene.
     */
    void load_scene(std::shared_ptr<cruelScene::Scene> scene);

    /**
     * \brief AddTasks
     *
     * Add new tasks to the RenderSession. This is for external tasks.
     */
    void add_new_task(std::unique_ptr<RenderTask> task);

    /**
     * \brief Prepare
     *
     * Run the preparation work for renderSession.
     */
    void prepare();

    /**
     * \brief Create render_pass
     *
     * Create a proper render pass according to the render properties.
     * Use subpasses. see [render pass best practice];
     */
    void prepare_render_pass();

    /**
     * \brief Draw
     *
     * Draw all render tasks, gui included.
     */
    void draw();

    /**
     * \brief CreateGuiStaff
     *
     * CreateGuiStaff creates swapchain, renderpass.
     */
    void CreateGuiStaff();

    /**
     * \brief CreateSceneStaff
     *
     * CreateSceneStaff creates render tasks for scene.
     */
    void CreateSceneStaff();

    /**
     * \brief DrawGui
     *
     * DrawGui draws gui. Records all draw commands related to the gui.
     */
    void DrawGui();

    /**
     * \brief DrawScene
     *
     * DrawScene records all draw commands related to the scene. Infact, it just call all
     * renderTasks' draw command.
     */
    void DrawScene();

    /**
     * \brief RenderGui
     *
     * RenderGui submit the draw commands of gui to graphic queue.
     */
    void RenderGui();

    /**
     * \brief RenderScene
     *
     * RenderGui submit the draw commands of gui to graphic queue.
     */
    void RenderScene();

    /**
     * \brief RenderFrame
     *
     * Draw a new frame.
     */
    void render_frame();

    LogicalDevice &get_device() const;
    Instance &     getInstance() const;
    Swapchain &    get_swapchain() const;
    Window &       get_window() const;
    VkSurfaceKHR & get_surface();
    RenderPass &   get_render_pass();
    CommandPool &  getCommandPool();
    void           set_session_id(u32 new_id);
    u32            get_session_id() const;
    Queue *        get_graphic_queue() const;
    Queue *        get_present_queue() const;
    float          getRenderTime() const;
    float          getFrameTime() const;

    /**
     * \brief SetGuiOverlay
     *
     * Set the guiOverlay for the RenderSession. If the pointer to guiOverlay is "null_ptr", the
     * session will not render the guioverlay.
     */
    void setGuiOverlay(std::shared_ptr<GuiOverlay> gui);

    std::shared_ptr<GuiOverlay> getGuiOverlay();

    bool is_session_alive();
    void createSemaphores();
    void destroySemaphores();

private:
    float render_time = 0;
    float frame_time  = 0;

    std::chrono::time_point<std::chrono::high_resolution_clock> render_time_marker{};
    std::chrono::time_point<std::chrono::high_resolution_clock> frame_time_marker{};

    u32 session_id = 0;
    u32 imgCount   = 3;
    // const int MAX_FRAMES_IN_FLIGHT = 2;

    VkSemaphore imageAvailableSemaphore = VK_NULL_HANDLE;
    VkSemaphore renderFinishedSemaphore = VK_NULL_HANDLE;
    VkFence     inFlightFences          = VK_NULL_HANDLE;
    u32         image_index             = 0;
    // std::vector<VkSemaphore> imageAvailableSemaphores{};
    // std::vector<VkSemaphore> renderFinishedSemaphores{};

    // std::vector<VkFence> inFlightFences{};
    // std::vector<VkFence> imagesInFlight{};
    // size_t               currentFrame = 0;

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

    //! \brief The renderpass of this session. Every session should have at least
    //! one render pass.
    std::unique_ptr<RenderPass> render_pass;

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

    // std::vector<std::unique_ptr<FrameBuffer>> frameBuffer;

    std::unique_ptr<CommandPool> commandPool;

    std::vector<std::unique_ptr<RenderFrame>> renderFrames{};

    // std::vector<std::reference_wrapper<CommandBuffer>> commandBuffers;

    std::vector<std::unique_ptr<RenderTask>> tasks;
};

void on_window_resize_cb(GLFWwindow *window, int width, int height);
} // namespace cruelRender
} // namespace cruelEngine