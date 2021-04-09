#include <algorithm>
#include <ctime>
#include <iostream>
#include <memory>
#include <vector>

#include "application/application.hpp"
#include "window/window.hpp"

#include "render/render_header.h"
#include "render/render_tasks/render_tasks.hpp"
#include "render/vkcommon.h"
#include "scene/scene_header.h"
#include "ui/gui.hpp"

u32  cruelEngine::Window::count       = 0;
bool cruelEngine::Window::glfw_inited = false;

using namespace cruelEngine;

void updateOverlay(void *gui);

class Render : cruelEngine::Application
{
public:
    Render(size_t size)
    {
#ifdef DEBUG
        std::cout << "frame time: " << frame_time << std::endl;
#endif

        render_context = std::make_unique<cruelEngine::cruelRender::RenderContext>(context_prop);
#ifdef DEBUG
        std::cout << "Render context created." << std::endl;
#endif

        scene = std::make_shared<cruelEngine::cruelScene::Scene>("scene");
#ifdef DEBUG
        std::cout << "Scene created." << std::endl;
#endif
        // scene->addObject(std::move(cruelEngine::cruelScene::SkyBox("Sky", scene->get_camera())));
#ifdef DEBUG
        std::cout << "Scene object created." << std::endl;
#endif

        for (size_t i = 0; i < size; i++)
        {
            render_context->add_session(properties.window_prop.title, properties);
            render_context->get_session(i).load_scene(scene);
            render_context->get_session(i).setGuiOverlay(
                std::make_shared<cruelGui::Gui>(render_context->get_session(i), guiFlags));
            render_context->get_session(i).getGuiOverlay()->setGuiOverlayUpdateCb(updateOverlay);
        }
#ifdef DEBUG
        std::cout << "Scene object loaded." << std::endl;
#endif
        render_context->draw();
    }

    ~Render()
    {
        render_context.reset();
    }

    void main_loop()
    {
        clock_t past_time   = clock();
        clock_t _frame_time = clock();
        while (render_context->is_context_alive())
        {
            glfwPollEvents();
            _frame_time = clock() - past_time;
            past_time   = clock();
            std::cout << "frame time: " << _frame_time * 1e-3
                      << " ms\tusleep: " << (frame_time - _frame_time) * 1e-3 << " ms" << std::endl;
            if (_frame_time < frame_time)
                usleep(frame_time - _frame_time);
            render_context->render_frame();
        }
    }

private:
    cruelGui::Gui::GuiUsageFlags guiFlags = cruelGui::Gui::GuiUsageFlagBits::GUI_ONLY_IMGUI |
                                            cruelGui::Gui::GuiUsageFlagBits::GUI_ENABLE_DOCKING;

    cruelEngine::cruelRender::RenderProp context_prop = {
        .appInfo                   = {.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO,
                    .pApplicationName = "Cruel Engine",
                    .pEngineName      = "Cruel Engine",
                    .apiVersion       = USED_VULKAN_API_VERSION},
        .validation                = true,
        .validationLayers          = {"VK_LAYER_KHRONOS_validation"},
        .enabledInstanceExtensions = {},
        .enabledDeviceExtensions   = {VK_KHR_SWAPCHAIN_EXTENSION_NAME},
        .features                  = {.fillModeNonSolid = true, .wideLines = true},
        .flags                     = {},
        .vsync                     = false,
        .overlay                   = false,
    };

    cruelEngine::cruelRender::SessionProp properties = {{"cruelEngine - Editor", 1280, 840, false}};

    // cruelEngine::cruelRender::SessionProp properties = {
    //     {"cruelEngine::Triangle::main Window", 720, 640, false}};

    std::shared_ptr<cruelEngine::cruelScene::Scene> scene;

    std::unique_ptr<cruelEngine::cruelRender::RenderContext> render_context;

    u32 frame_time = 10e3;
};

void updateOverlay(void *gui)
{
    auto uiOverlay = (cruelGui::Gui *) gui;
    if (uiOverlay == nullptr)
        std::cerr << "[Render::main_loop::updateOverlay] updating guiOverlay" << std::endl;

    static bool show_magic_dick  = true;
    static bool show_demo_window = true;
    static bool pause_record     = false;
    static bool show_main_menu   = true;
    if (uiOverlay->getGuiFlags() & cruelGui::Gui::GuiUsageFlagBits::GUI_ENABLE_DOCKING)
    {
        ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus |
            (ImGuiWindowFlags_MenuBar * show_main_menu) | ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove;

        ImGuiViewport *viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y));
        ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, viewport->Size.y));
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("RootWindow", 0, window_flags);
        ImGui::PopStyleVar(3);

        ImGuiID dockspace_id = ImGui::GetID("RootWindow");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
    }

    if (show_main_menu)
    {
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File"))
        {
            ImGui::MenuItem("(demo menu)", NULL, false, false);
            if (ImGui::MenuItem("New"))
            {}
            if (ImGui::MenuItem("Open", "Ctrl+O"))
            {}
            if (ImGui::MenuItem("Magic Dick"))
            {
                show_magic_dick = !show_magic_dick;
            }

            if (ImGui::BeginMenu("Open Recent"))
            {
                ImGui::MenuItem("fish_hat.c");
                ImGui::MenuItem("fish_hat.inl");
                ImGui::MenuItem("fish_hat.h");
                if (ImGui::BeginMenu("More.."))
                {
                    ImGui::MenuItem("Hello");
                    ImGui::MenuItem("Sailor");
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if (ImGui::MenuItem("Save", "Ctrl+S"))
            {}
            if (ImGui::MenuItem("Save As.."))
            {}

            ImGui::Separator();
            if (ImGui::BeginMenu("Options"))
            {
                static bool enabled = true;
                ImGui::MenuItem("Enabled", "", &enabled);
                ImGui::BeginChild("child", ImVec2(0, 60), true);
                for (int i = 0; i < 10; i++)
                    ImGui::Text("Scrolling Text %d", i);
                ImGui::EndChild();
                static float f = 0.5f;
                static int   n = 0;
                ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
                ImGui::InputFloat("Input", &f, 0.1f);
                ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Colors"))
            {
                float sz = ImGui::GetTextLineHeight();
                for (int i = 0; i < ImGuiCol_COUNT; i++)
                {
                    const char *name = ImGui::GetStyleColorName((ImGuiCol) i);
                    ImVec2      p    = ImGui::GetCursorScreenPos();
                    ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz),
                                                              ImGui::GetColorU32((ImGuiCol) i));
                    ImGui::Dummy(ImVec2(sz, sz));
                    ImGui::SameLine();
                    ImGui::MenuItem(name);
                }
                ImGui::EndMenu();
            }

            // Here we demonstrate appending again to the "Options" menu (which we already created
            // above) Of course in this demo it is a little bit silly that this function calls
            // BeginMenu("Options") twice. In a real code-base using it would make senses to use
            // this feature from very different code locations.
            if (ImGui::BeginMenu("Options")) // <-- Append!
            {
                static bool b = true;
                ImGui::Checkbox("SomeOption", &b);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Disabled", false)) // Disabled
            {
                IM_ASSERT(0);
            }
            if (ImGui::MenuItem("Checked", NULL, true))
            {}
            if (ImGui::MenuItem("Quit", "Alt+F4"))
            {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "CTRL+Z"))
            {}
            if (ImGui::MenuItem("Redo", "CTRL+Y", false, false))
            {} // Disabled item
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "CTRL+X"))
            {}
            if (ImGui::MenuItem("Copy", "CTRL+C"))
            {}
            if (ImGui::MenuItem("Paste", "CTRL+V"))
            {}
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    if (show_magic_dick)
    {
        ImGui::Begin("Magic Dick", &show_magic_dick, ImGuiWindowFlags_NoResize);
        uiOverlay->checkBox("Show demo", &show_demo_window);
        ImGui::SameLine();
        uiOverlay->checkBox("Pause Record", &pause_record);

        uiOverlay->text("Application average %.3f ms/frame (%.1f FPS)",
                        1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        static float frame_times[240]     = {0};
        static float rendering_times[240] = {0};
        static int   offset               = 0;
        if (pause_record == false)
        {
            frame_times[offset]     = uiOverlay->getSession().getFrameTime();
            rendering_times[offset] = uiOverlay->getSession().getRenderTime();
            offset                  = (offset + 1) % IM_ARRAYSIZE(frame_times);
        }

        {
            float average = 0.0f;
            for (int n = 0; n < IM_ARRAYSIZE(frame_times); n++)
                average += frame_times[n];
            average /= (float) IM_ARRAYSIZE(frame_times);
            char overlay[32];
            sprintf(overlay, "avg %.3f ms/frame", average);
            ImGui::PlotLines("Frame time", frame_times, IM_ARRAYSIZE(frame_times), offset, overlay,
                             FLT_MAX, FLT_MAX, ImVec2(0, 120.0f));
        }
        {
            float average = 0.0f;
            for (int n = 0; n < IM_ARRAYSIZE(rendering_times); n++)
                average += rendering_times[n];
            average /= (float) IM_ARRAYSIZE(rendering_times);
            char overlay[32];
            sprintf(overlay, "avg %.3f ms/frame", average);
            ImGui::PlotLines("Render time", rendering_times, IM_ARRAYSIZE(rendering_times), offset,
                             overlay, FLT_MAX, FLT_MAX, ImVec2(0, 120.0f));
        }
        ImGui::End();
    }

    if (show_demo_window)
    {
        ImGui::ShowDemoWindow(&show_demo_window);
    }

    if (uiOverlay->getGuiFlags() & cruelGui::Gui::GuiUsageFlagBits::GUI_ENABLE_DOCKING)
    {
        ImGui::End();
    }

    uiOverlay->requireUpdate();
}

int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("Usage: \n"
               "  <%s>   [num of windows]\n",
               argv[0]);
        return -1;
    }

    Render(atoi(argv[1])).main_loop();

    return 0;
}