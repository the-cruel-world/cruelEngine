#pragma once
#include "vkcommon.h"
//! cant change the order of vkcommon and window/window it's so weird.
#include "../window/window.hpp"

#define MAIN_VER 0
#define MID_VER 0
#define END_VER 1

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

// Todo
// 1. The rendersession should support headless render.
// 2. Session should support multi swapchains/renderpass.

struct SessionProp
{
    WindowProp window_prop;
};

class RenderSession
{
public:
    RenderSession(Instance &instance, LogicalDevice &device, SessionProp &session_properties);

    RenderSession(const RenderSession &) = delete;

    RenderSession &operator=(const RenderSession &) = delete;

    ~RenderSession();

    /**
     * \brief update the swapchain
     * When the a window_resize event is captured, execute this function
     * to rebuild a new swapchain.
     */
    void update_swapchain();
    void recreate_swapchain()
    {}

    void load_scene(std::shared_ptr<cruelScene::Scene> scene);

    void add_new_task(std::unique_ptr<RenderTask> task);

    void setGuiOverlay(std::shared_ptr<GuiOverlay> gui);

    std::shared_ptr<GuiOverlay> getGuiOverlay();

    void prepare();

    /**
     * Create a proper render pass according to the render properties.
     * Use subpasses. see [render pass best practice];
     */
    void prepare_render_pass();

    void draw();

    void render_frame();

    LogicalDevice &get_device() const;

    Instance &getInstance() const
    {
        return instance;
    }

    Swapchain &get_swapchain() const;

    Window &get_window() const;

    VkSurfaceKHR &get_surface();

    RenderPass &get_render_pass();

    bool is_session_alive();

    void createSemaphores();
    void destroySemaphores();

    CommandPool &getCommandPool();

    void set_session_id(u32 new_id);
    u32  get_session_id() const;

    Queue *get_graphic_queue() const;
    Queue *get_present_queue() const;

    float getRenderTime() const
    {
        return render_time;
    }

    float getFrameTime() const
    {
        return frame_time;
    }

private:
    float render_time = 0;
    float frame_time  = 0;

    std::chrono::time_point<std::chrono::high_resolution_clock> render_time_marker{};
    std::chrono::time_point<std::chrono::high_resolution_clock> frame_time_marker{};

    u32 session_id = 0;

    u32 imgCount = 5;

    const int MAX_FRAMES_IN_FLIGHT = 2;

    VkSemaphore              imageAvailableSemaphore = VK_NULL_HANDLE;
    VkSemaphore              renderFinishedSemaphore = VK_NULL_HANDLE;
    std::vector<VkSemaphore> imageAvailableSemaphores{};
    std::vector<VkSemaphore> renderFinishedSemaphores{};

    std::vector<VkFence> inFlightFences{};
    std::vector<VkFence> imagesInFlight{};
    size_t               currentFrame = 0;

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

    std::vector<std::unique_ptr<FrameBuffer>> frameBuffer;

    std::unique_ptr<CommandPool> commandPool;

    std::vector<std::reference_wrapper<CommandBuffer>> commandBuffers;

    std::vector<std::unique_ptr<RenderTask>> tasks;
};

void on_window_resize_cb(GLFWwindow *window, int width, int height);
} // namespace cruelRender
} // namespace cruelEngine