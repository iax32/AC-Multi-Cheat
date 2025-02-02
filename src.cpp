#include "settings.h"
#include "utilities.h"
#include "geometry.h"
#include "render.h"
#include <vector>


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"



#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) &&a !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

// This example can also compile and run with Emscripten! See 'Makefile.emscripten' for details.
#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

extern HANDLE hProcess = 0;

bool menu_visible = true;
bool enable_enemy_esp = false;
bool enable_enemy_esp_corner = false;
bool enable_team_esp = false;
bool enable_team_esp_corner = false;
bool enable_enemy_healthbar = false;
bool enable_enemy_armorbar = false;
bool enable_team_healthbar = false;
bool enable_team_armorbar = false;
float color_team_box[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
float color_enemy_box[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
float color_team_snaplines[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
float color_enemy_snaplines[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
bool enable_enemy_snaplines = false;
bool enable_team_snaplines = false;
bool enable_aimbot = false;

extern float nearest_enemy = -1.0f;
extern float new_yaw = 0.0f;
extern float new_pitch = 0.0f;



// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
    
    const char* glsl_version = "#version 130";

    HWND hWnd = GetConsoleWindow();
    ShowWindow(hWnd, SW_HIDE);
    GLFWmonitor* Monitor = glfwGetPrimaryMonitor();
    if (!Monitor)
    {
        return 0;
    }
    int height = glfwGetVideoMode(Monitor)->height;
    int width = glfwGetVideoMode(Monitor)->width;
    glfwWindowHint(GLFW_FLOATING, true);
    glfwWindowHint(GLFW_MAXIMIZED, true);
    glfwWindowHint(GLFW_RESIZABLE, false);
    glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, true);


    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(width, height, "Assault Cube Multi Cheat", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwSetWindowAttrib(window, GLFW_DECORATED, false);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    SetupImGuiStyle();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    DWORD procId = GetProcId(L"ac_client.exe");
    uintptr_t moduleBase = GetModuleBaseAddr(procId, L"ac_client.exe");
    hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, procId);
    Entity Player;
    std::vector<Entity> entity(32);
    Player.baseaddress = Memory::RPM<uintptr_t>(moduleBase + 0x10f4f4);

    

    // Main loop
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not attempt to do a fopen() of the imgui.ini file.
    // You may manually call LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT);
        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        if (GetAsyncKeyState(VK_INSERT) & 1)
        {
            menu_visible = !menu_visible;

            if (menu_visible)
            {
                ShowMenu(window);
            }
            else
            {
                HideMenu(window);
            }
        }

        if (menu_visible)
        {
            ImGui::SetNextWindowSize(ImVec2(500, 0));
            ImGui::Begin("AC Multi Cheat", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

            // Haupttitel
            ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "ESP Settings");
            ImGui::Separator();

            // Spalten für Team ESP und Enemy ESP
            ImGui::Columns(2, nullptr, false); // Zwei Spalten, ohne Resizing

            ImGui::Text("Team ESP"); ImGui::NextColumn();
            ImGui::Text("Enemy ESP"); ImGui::NextColumn();

            ImGui::Checkbox("T Box", &enable_team_esp); ImGui::NextColumn();
            ImGui::Checkbox("E Box", &enable_enemy_esp); ImGui::NextColumn();

            ImGui::Checkbox("T Corner Box", &enable_team_esp_corner); ImGui::NextColumn();
            ImGui::Checkbox("E Corner Box", &enable_enemy_esp_corner); ImGui::NextColumn();

            ImGui::Checkbox("T Health Bar", &enable_team_healthbar); ImGui::NextColumn();
            ImGui::Checkbox("E Health Bar", &enable_enemy_healthbar); ImGui::NextColumn();

            ImGui::Checkbox("T Armor Bar", &enable_team_armorbar); ImGui::NextColumn();
            ImGui::Checkbox("E Armor Bar", &enable_enemy_armorbar); ImGui::NextColumn();

            // Zurück zu einer Spalte
            ImGui::Columns(1);
            ImGui::Spacing();

            // Snapline Optionen
            ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "Snaplines");
            ImGui::Separator();

            // Checkboxen für Snaplines
            ImGui::Columns(2, nullptr, false);
            ImGui::Checkbox("Team Snaplines", &enable_team_snaplines); ImGui::NextColumn();
            ImGui::Checkbox("Enemy Snaplines", &enable_enemy_snaplines); ImGui::NextColumn();

            // Zurück zu einer Spalte
            ImGui::Columns(1);
            ImGui::Spacing();

            // Farboptionen
            ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "Colors");
            ImGui::Separator();

            // Farben für Team und Enemy ESP
            ImGui::Columns(2, nullptr, false);
            ImGui::ColorEdit4("Team ESP", color_team_box, ImGuiColorEditFlags_NoInputs); ImGui::NextColumn();
            ImGui::ColorEdit4("Enemy ESP", color_enemy_box, ImGuiColorEditFlags_NoInputs); ImGui::NextColumn();

            ImGui::ColorEdit4("Team Snaplines", color_team_snaplines, ImGuiColorEditFlags_NoInputs); ImGui::NextColumn();
            ImGui::ColorEdit4("Enemy Snaplines", color_enemy_snaplines, ImGuiColorEditFlags_NoInputs); ImGui::NextColumn();

            // Zurück zu einer Spalte
            ImGui::Columns(1);
            ImGui::Spacing();


            // Snapline Optionen
            ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "Aimbot");
            ImGui::Separator();
            ImGui::Checkbox("Enemy Aimbot", &enable_aimbot); ImGui::NextColumn();

            // Ende des Menüs
            ImGui::End();

        }

        getEntity(Player);
        nearest_enemy = -1.0f;
        new_yaw = 0.0f;
        new_pitch = 0.0f;
        Player.Matrix = Memory::RPM<MVPMatrix>(moduleBase + offsets::mvp_matrix);
        uintptr_t listEntry = Memory::RPM<uintptr_t>(moduleBase + offsets::entitylist);
        for (unsigned int i = 0; i <= 32; i++)
        {
            entity[i].baseaddress = Memory::RPM<uintptr_t>(listEntry + i * 0x4);
            if (entity[i].baseaddress == NULL)
                continue;
            getEntity(entity[i]);
            if (entity[i].health <= 0 || entity[i].health > 100)
                continue;

            Vec2 HeadCoords;
            Vec2 FeetCoords;

            if (!WorldToScreen(entity[i].Origin, FeetCoords, Player.Matrix.Matrix))
                continue;
            if (!WorldToScreen(entity[i].HeadOrigin, HeadCoords, Player.Matrix.Matrix))
                continue;

            bool is_enemy = (Player.team != entity[i].team);

            if (is_enemy)
            {
                if (enable_enemy_esp_corner)
                    DrawCornerOutline(FeetCoords, HeadCoords, color_enemy_box);
                if (enable_enemy_esp)
                    DrawBox(FeetCoords, HeadCoords, color_enemy_box);
                if (enable_enemy_snaplines)
                    DrawSnapLines(FeetCoords, color_enemy_snaplines);
                if (enable_enemy_healthbar)
                    DrawHealthBar(FeetCoords, HeadCoords, entity[i].health);
                if (enable_enemy_armorbar)
                    DrawArmorBar(FeetCoords, HeadCoords, entity[i].armor);

                if (enable_aimbot)
                    CalculateNewAngles(Player, entity[i]);

            }
            else
            {
                if (enable_team_esp_corner)
                    DrawCornerOutline(FeetCoords, HeadCoords, color_team_box);
                if (enable_team_esp)
                    DrawBox(FeetCoords, HeadCoords, color_team_box);
                if (enable_team_snaplines)
                    DrawSnapLines(FeetCoords, color_team_snaplines);
                if (enable_team_healthbar)
                    DrawHealthBar(FeetCoords, HeadCoords, entity[i].health);
                if (enable_team_armorbar)
                    DrawArmorBar(FeetCoords, HeadCoords, entity[i].armor);
            }

        }

        Memory::WPM<int>(Player.baseaddress + offsets::mag_assaultRifle, 1337);
        Memory::WPM<int>(Player.baseaddress + offsets::health, 1337);

        if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && enable_aimbot && nearest_enemy != -1.0f)
        {
            Memory::WPM<Vec3>(Player.baseaddress + offsets::yaw, { new_yaw, new_pitch, 0 });
        }



        // Rendering
        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
