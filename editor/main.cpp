#include <algorithm>
#include <ctime>
#include <iostream>
#include <memory>
#include <vector>

#include "../src/application/application.hpp"
#include "../src/window/window.hpp"

#include "../src/render/render_header.h"
#include "../src/render/vkcommon.h"
#include "../src/scene/scene_header.h"
#include "../src/ui/gui.hpp"

u32                cruelEngine::Window::count       = 0;
bool               cruelEngine::Window::glfw_inited = false;
TextEditor         editor;
static const char *fileToEdit = "main.cpp";

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

        scene = std::make_shared<cruelEngine::cruelScene::Scene>();
#ifdef DEBUG
        std::cout << "Scene created." << std::endl;
#endif
        // scene->addObject(
        //     std::make_shared<cruelEngine::cruelScene::SkyBox>("Sky", scene->get_camera()));
#ifdef DEBUG
        std::cout << "Scene object created." << std::endl;
#endif

        for (size_t i = 0; i < size; i++)
        {
            render_context->add_session(properties.window_prop.title, properties);
            render_context->get_session(i).load_scene(scene);
            render_context->get_session(i).setGuiOverlay(
                std::make_shared<cruelGui::Gui>(render_context->get_session(i), guiFlags));
#ifdef DEBUG
            std::cout << "loading gui overlay" << std::endl;
#endif
            render_context->get_session(i).getGuiOverlay()->setGuiOverlayUpdateCb(updateOverlay);
#ifdef DEBUG
            std::cout << "loaded gui overlay" << std::endl;
#endif
        }
#ifdef DEBUG
        std::cout << "Scene object loaded." << std::endl;
#endif

        {
            // editor.SetPalette(TextEditor::GetLightPalette());
            std::ifstream t(fileToEdit);
            if (t.good())
            {
                std::string str((std::istreambuf_iterator<char>(t)),
                                std::istreambuf_iterator<char>());
                editor.SetText(str);
            }
        }
        // TextEditor::ErrorMarkers markers;
        // markers.insert(std::make_pair<int, std::string>(
        //     6, "Example error here:\nInclude file not found: \"TextEditor.h\""));
        // markers.insert(std::make_pair<int, std::string>(41, "Another example error"));
        // editor.SetErrorMarkers(markers);

        render_context->draw();
    }

    ~Render()
    {
        render_context.reset();
    }

    void main_loop()
    {
        static clock_t past_time   = clock();
        static clock_t _frame_time = clock();
        while (render_context->is_context_alive())
        {
            glfwPollEvents();
            _frame_time = clock() - past_time;
            past_time   = clock();
            // std::cout << "frame time: " << _frame_time * 1e-3
            //           << " ms\tusleep: " << (frame_time - _frame_time) * 1e-3 << " ms" <<
            //           std::endl;
            if (_frame_time < frame_time)
                usleep(frame_time - _frame_time);
            render_context->render_frame();
        }
    }

private:
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

    cruelGui::Gui::GuiUsageFlags guiFlags = cruelGui::Gui::GuiUsageFlagBits::GUI_ENABLE_DOCKING |
                                            cruelGui::Gui::GuiUsageFlagBits::GUI_ENABLE_IMPLOT |
                                            cruelGui::Gui::GuiUsageFlagBits::GUI_ENABLE_MARKDOWN;
    cruelEngine::cruelRender::SessionProp properties = {{"cruelEngine - Editor", 1280, 840, false}};

    std::shared_ptr<cruelEngine::cruelScene::Scene> scene;

    std::unique_ptr<cruelEngine::cruelRender::RenderContext> render_context;

    u32 frame_time = 16e3;
};

void updateOverlay(void *gui)
{
    auto uiOverlay = (cruelGui::Gui *) gui;
    if (uiOverlay == nullptr)
        std::cerr << "[Render::main_loop::updateOverlay] updating guiOverlay" << std::endl;

    static bool show_magic_dick    = true;
    static bool show_demo_window   = true;
    static bool pause_record       = false;
    static bool show_editor_window = true;
    static bool show_main_menu     = true;

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

    if (uiOverlay->getGuiFlags() & cruelGui::Gui::GuiUsageFlagBits::GUI_ENABLE_DOCKING)
    {
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
        ImGui::SameLine();
        uiOverlay->checkBox("editor window", &show_editor_window);

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
        ImPlot::ShowDemoWindow(&show_demo_window);
    }

    if (show_editor_window)
    {
        auto cpos = editor.GetCursorPosition();
        ImGui::Begin("Text Editor", &show_editor_window,
                     ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_MenuBar);
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Save", "Ctrl-S", nullptr, true))
                {
                    auto textToSave = editor.GetText();
                    {
                        std::ofstream t(fileToEdit);
                        t << textToSave;
                        t.close();
                    }
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                bool ro = editor.IsReadOnly();
                if (ImGui::MenuItem("Read-only mode", nullptr, &ro))
                    editor.SetReadOnly(ro);
                ImGui::Separator();

                if (ImGui::MenuItem("Undo", "ALT-Backspace", nullptr, !ro && editor.CanUndo()))
                    editor.Undo();
                if (ImGui::MenuItem("Redo", "Ctrl-Y", nullptr, !ro && editor.CanRedo()))
                    editor.Redo();

                ImGui::Separator();

                if (ImGui::MenuItem("Copy", "Ctrl-C", nullptr, editor.HasSelection()))
                    editor.Copy();
                if (ImGui::MenuItem("Cut", "Ctrl-X", nullptr, !ro && editor.HasSelection()))
                    editor.Cut();
                if (ImGui::MenuItem("Delete", "Del", nullptr, !ro && editor.HasSelection()))
                    editor.Delete();
                if (ImGui::MenuItem("Paste", "Ctrl-V", nullptr,
                                    !ro && ImGui::GetClipboardText() != nullptr))
                    editor.Paste();

                ImGui::Separator();

                if (ImGui::MenuItem("Select all", nullptr, nullptr))
                    editor.SetSelection(TextEditor::Coordinates(),
                                        TextEditor::Coordinates(editor.GetTotalLines(), 0));

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Dark palette"))
                    editor.SetPalette(TextEditor::GetDarkPalette());
                if (ImGui::MenuItem("Light palette"))
                    editor.SetPalette(TextEditor::GetLightPalette());
                if (ImGui::MenuItem("Retro blue palette"))
                    editor.SetPalette(TextEditor::GetRetroBluePalette());
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        ImGui::Text("%6d/%-6d %6d lines  | %s | %s | %s | %s", cpos.mLine + 1, cpos.mColumn + 1,
                    editor.GetTotalLines(), editor.IsOverwrite() ? "Ovr" : "Ins",
                    editor.CanUndo() ? "*" : " ", editor.GetLanguageDefinition().mName.c_str(),
                    fileToEdit);
        editor.Render("TextEditor");
        ImGui::End();
    }

    uiOverlay->requireUpdate();
    ImGui::End();
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