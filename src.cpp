#include "settings.h"
#include "utilities.h"
#include "geometry.h"
#include "render.h"
#include <vector>
#include <iostream>
#include <string>


#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#define GLFW_EXPOSE_NATIVE_WIN32 // Define before including glfw3native.h
#include <GLFW/glfw3native.h>   // Include GLFW native header



#define GL_SILENCE_DEPRECATION
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h> // Will drag system OpenGL headers

// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
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
bool enable_aimbot_debug = false;
bool debug_vectors = false, debug_aimbot = false, debug_entities = false;

static int esp_mode = 0;
extern float nearest_enemy = -1.0f;
extern float new_yaw = 0.0f;
extern float new_pitch = 0.0f;


bool activatedLastPos = false;

GLuint fontBase;

Vec2 playerHeadCoords;
Vec2 playerFeetCoords;
Vec2 HeadCoords;
Vec2 FeetCoords;
Vec2 distanceNDC;

std::string str;
std::string str1;

Vec2 screenCorners2[4];
Vec2 boundingBoxesNdc[8];

Vec2 gizmoNdc[3];
Vec2 gizmoPlayerNdc[3];


// Main code
int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;
   
    if (glfwGetCurrentContext() == NULL) {
        std::cerr << "Error: OpenGL context is not initialized!" << std::endl;
    }

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
    Vec3 PreviousPosition;

    // Get device context
    HDC hdc = GetDC(glfwGetWin32Window(window));
    fontBase = createBitmapFont(hdc, "Consolas", 24);

    float menu_alpha;
    float menu_background_color[4];
    bool esp_show_distance = false;
    bool esp_show_names = false;
    float aimbot_smoothness;
    float aimbot_fov;
    bool aimbot_target_head = false;
    bool aimbot_target_chest = false;
    bool enable_bunnyhop = false;
    bool enable_no_recoil = false;
    bool enable_no_spread = false;
    bool enable_triggerbot = false;
    bool enable_radar = false;
    bool enable_crosshair = false;
    float crosshair_size;
    float color_crosshair[4];
    float color_radar[4];


    // Main loope
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
            ImGui::SetNextWindowSize(ImVec2(850, 700));
            ImGui::Begin("AC Multi Cheat", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize);

            if (ImGui::BeginTabBar("CheatTabs"))
            {
                if (ImGui::BeginTabItem("ESP"))
                {
                    ImGui::BeginChild("ESP_Settings", ImVec2(400, 150), true);
                    ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "ESP Settings");
                    const char* esp_modes[] = { "Off", "Box", "3D Box", "Corner Box", "Skeleton", "Glow" };
                    ImGui::Combo("ESP Mode", &esp_mode, esp_modes, IM_ARRAYSIZE(esp_modes));
                    ImGui::Checkbox("Show Distance", &esp_show_distance);
                    ImGui::Checkbox("Show Names", &esp_show_names);
                    ImGui::EndChild();

                    ImGui::SameLine();

                    ImGui::BeginChild("ESP_Checkboxes", ImVec2(400, 150), true);
                    ImGui::Columns(2, nullptr, false);
                    ImGui::Text("Team ESP"); ImGui::NextColumn();
                    ImGui::Text("Enemy ESP"); ImGui::NextColumn();
                    ImGui::Checkbox("Team Box", &enable_team_esp); ImGui::NextColumn();
                    ImGui::Checkbox("Enemy Box", &enable_enemy_esp); ImGui::NextColumn();
                    ImGui::Checkbox("Team Health Bar", &enable_team_healthbar); ImGui::NextColumn();
                    ImGui::Checkbox("Enemy Health Bar", &enable_enemy_healthbar); ImGui::NextColumn();
                    ImGui::Checkbox("Team Armor Bar", &enable_team_armorbar); ImGui::NextColumn();
                    ImGui::Checkbox("Enemy Armor Bar", &enable_enemy_armorbar); ImGui::NextColumn();

                    ImGui::Checkbox("Team Snaplines", &enable_team_snaplines); ImGui::NextColumn();
                    ImGui::Checkbox("Enemy Snaplines", &enable_enemy_snaplines); ImGui::NextColumn();
                    ImGui::EndChild();

                    ImGui::NewLine();

                    ImGui::BeginChild("Color_Settings", ImVec2(400, 150), true);
                    ImGui::TextColored(ImVec4(0.2f, 0.8f, 1.0f, 1.0f), "Colors");

                    // Farben für Team und Enemy ESP
                    ImGui::Columns(2, nullptr, false);
                    ImGui::ColorEdit4("Team ESP", color_team_box, ImGuiColorEditFlags_NoInputs); ImGui::NextColumn();
                    ImGui::ColorEdit4("Enemy ESP", color_enemy_box, ImGuiColorEditFlags_NoInputs); ImGui::NextColumn();

                    ImGui::ColorEdit4("Team Snaplines", color_team_snaplines, ImGuiColorEditFlags_NoInputs); ImGui::NextColumn();
                    ImGui::ColorEdit4("Enemy Snaplines", color_enemy_snaplines, ImGuiColorEditFlags_NoInputs); ImGui::NextColumn();

                    // Zurück zu einer Spalte
                    ImGui::Columns(1);
                    ImGui::Separator();

                    ImGui::ColorEdit4("Crosshair", color_crosshair, ImGuiColorEditFlags_NoInputs);
                    ImGui::ColorEdit4("Radar", color_radar, ImGuiColorEditFlags_NoInputs);
                    ImGui::EndChild();

                    ImGui::SameLine();

                    ImGui::BeginChild("Radar_Settings", ImVec2(400, 150), true);
                    ImGui::TextColored(ImVec4(0.8f, 0.3f, 1.0f, 1.0f), "Radar");
                    ImGui::Checkbox("Enable Radar", &enable_radar);
                    ImGui::EndChild();

                    ImGui::NewLine();

                    ImGui::BeginChild("Crosshair_Settings", ImVec2(400, 150), true);
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Crosshair");
                    ImGui::Checkbox("Enable Custom Crosshair", &enable_crosshair);
                    ImGui::SliderFloat("Crosshair Size", &crosshair_size, 5.0f, 50.0f);
                    ImGui::EndChild();

                    ImGui::SameLine();
                    
                    ImGui::BeginChild("Debugging", ImVec2(400, 150), true);
                    // Debugging Options
                    ImGui::TextColored(ImVec4(0.9f, 0.3f, 0.3f, 1.0f), "Debugging");
                    ImGui::Separator();
                    ImGui::Checkbox("Show Direction Vectors", &debug_vectors);
                    ImGui::Checkbox("Enable Aimbot Debug", &debug_aimbot);
                    ImGui::Checkbox("Show Entity Info", &debug_entities);
                    ImGui::EndChild();


                    if (debug_entities)
                    {
                        ImVec2 section_pos = ImGui::GetCursorScreenPos(); // Get position
                        ImVec2 section_size = ImVec2(ImGui::GetContentRegionAvail().x, 180); // Set size (adjust height as needed)

                        ImGui::GetWindowDrawList()->AddRectFilled(
                            section_pos,
                            ImVec2(section_pos.x + section_size.x, section_pos.y + section_size.y),
                            IM_COL32(20, 20, 20, 200), // Background color (RGBA)
                            5.0f // Optional: Rounded corners
                        );
                        ImGui::TextColored(ImVec4(0.5f, 1.0f, 0.5f, 1.0f), "Entities");
                        ImGui::Separator();
                        for (size_t i = 0; i < entity.size(); i++)
                        {
                            std::string label = "Entity " + std::to_string(i);
                            if (ImGui::CollapsingHeader(label.c_str()))
                            {
                                ImGui::Text("Health: %d", entity[i].health);
                                ImGui::Text("Armor: %d", entity[i].armor);
                                ImGui::Text("Team: %d", entity[i].team);
                            }
                        }
                    }


                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Aimbot"))
                {
                    ImGui::BeginChild("Aimbot_Settings", ImVec2(400, 200), true);
                    ImGui::TextColored(ImVec4(1.0f, 0.2f, 0.2f, 1.0f), "Aimbot");
                    ImGui::Checkbox("Enable Aimbot", &enable_aimbot);
                    ImGui::SliderFloat("FOV", &aimbot_fov, 0.0f, 180.0f, "%.1f");
                    ImGui::SliderFloat("Smoothness", &aimbot_smoothness, 0.0f, 1.0f, "%.2f");
                    ImGui::Checkbox("Target Head", &aimbot_target_head);
                    ImGui::Checkbox("Target Chest", &aimbot_target_chest);
                    ImGui::EndChild();

                    ImGui::BeginChild("Triggerbot_Settings", ImVec2(400, 200), true);
                    ImGui::TextColored(ImVec4(0.8f, 0.5f, 0.2f, 1.0f), "Triggerbot");
                    ImGui::Checkbox("Enable Triggerbot", &enable_triggerbot);
                    ImGui::EndChild();


                    ImGui::EndTabItem();
                }

                if (ImGui::BeginTabItem("Movement"))
                {
                    ImGui::BeginChild("Movement_Settings", ImVec2(400, 200), true);
                    ImGui::TextColored(ImVec4(0.3f, 1.0f, 0.3f, 1.0f), "Movement");
                    ImGui::Checkbox("Bunnyhop", &enable_bunnyhop);
                    ImGui::Checkbox("No Recoil", &enable_no_recoil);
                    ImGui::Checkbox("No Spread", &enable_no_spread);
                    ImGui::EndChild();
                    ImGui::EndTabItem();
                }

                ImGui::EndTabBar();
            }

            ImGui::End();
        }



        Player.getEntity();
        nearest_enemy = -1.0f;
        new_yaw = 0.0f;
        new_pitch = 0.0f;
        Player.Matrix = Memory::RPM<MVPMatrix>(moduleBase + offsets::mvp_matrix);
        uintptr_t listEntry = Memory::RPM<uintptr_t>(moduleBase + offsets::entitylist);

        

        for (unsigned int i = 0; i < 32; i++)  // Fix loop condition (0 <= 32 can cause unnecessary iterations)
        {

            entity[i].baseaddress = Memory::RPM<uintptr_t>(listEntry + i * 0x4);

            

            if (entity[i].baseaddress == NULL)
                continue;


            entity[i].getEntity();


            if (entity[i].health <= 0 || entity[i].health > 100)
                continue;



			if (activatedLastPos)
			{
				Player.Origin = PreviousPosition;
			}

            float triangleColor[4] = {1.0f, 1.0f, 1.0f, 1.0f};
            Vec3 corners2[4] =
            {
                Player.Origin, // 0
                entity[i].Origin, // 1
                {Player.Origin.X, entity[i].Origin.Y, entity[i].Origin.Z}, // 2
                entity[i].HeadOrigin // 3
            };



            WorldToScreen(corners2[2], screenCorners2[2], Player.Matrix.Matrix);
            WorldToScreenPlayer(Player.Origin, screenCorners2[0], Player.Matrix.Matrix);
            WorldToScreenPlayer(Player.Origin, playerFeetCoords, Player.Matrix.Matrix);


            // Transform Feet and Head Positions
            if (!WorldToScreen(entity[i].Origin, FeetCoords, Player.Matrix.Matrix))
                continue;
            if (!WorldToScreen(entity[i].HeadOrigin, HeadCoords, Player.Matrix.Matrix))
                continue;
			

            /*
            for (int j = 0; j < 8; j++) {
                if (!WorldToScreen(entity[i].boundingBoxes[j], entity[i].boundingBoxesNdc[i], Player.Matrix.Matrix)) {
                    std::cout << "Corner " << j << " failed to transform!" << std::endl;
                    allCornersValid = false;
                    break;  // Skip rendering if any point is off-screen
                }
            }
            */
            
            

            float boxColor[4] = { 0.0f, 1.0f, 1.0f, 1.0f }; // Red box

            Vec3 boundingBoxes[8] =
            {
                  {entity[i].Origin.X + 2, entity[i].Origin.Y + 2, entity[i].Origin.Z},
                  {entity[i].Origin.X + 2, entity[i].Origin.Y - 2, entity[i].Origin.Z},

                  {entity[i].Origin.X - 2, entity[i].Origin.Y - 2, entity[i].Origin.Z},
                  {entity[i].Origin.X - 2, entity[i].Origin.Y + 2, entity[i].Origin.Z},

                  {entity[i].Origin.X + 2, entity[i].Origin.Y + 2, entity[i].HeadOrigin.Z},
                  {entity[i].Origin.X + 2, entity[i].Origin.Y - 2, entity[i].HeadOrigin.Z},

                  {entity[i].Origin.X - 2, entity[i].Origin.Y - 2, entity[i].HeadOrigin.Z},
                  {entity[i].Origin.X - 2, entity[i].Origin.Y + 2, entity[i].HeadOrigin.Z},
            };

            
            bool allCornersValid = true;

            for (int j = 0; j < 8; j++) {
                if (!WorldToScreen(boundingBoxes[j], boundingBoxesNdc[j], Player.Matrix.Matrix)) {
                    std::cout << "Corner " << j << " failed to transform!" << std::endl;
                    allCornersValid = false;
                }
            }
            
            // Determine if the entity is an enemy
            bool is_enemy = (Player.team != entity[i].team);

            if (is_enemy) {
                glLineWidth(1.0f);
                switch (esp_mode)
                {
                case 1:
                    if (enable_enemy_esp)
                        DrawBox(FeetCoords, HeadCoords, color_enemy_box);
                    break;
                case 2:
                    if (allCornersValid && enable_enemy_esp)
                        DrawBox3D(boundingBoxesNdc, color_enemy_box);
                    break;
                case 3:
                    if (enable_enemy_esp)
                        DrawCornerOutline(FeetCoords, HeadCoords, color_enemy_box);
                    break;
                }

                if (enable_enemy_snaplines)
                    DrawSnapLines(FeetCoords, color_enemy_snaplines);
                if (enable_enemy_healthbar)
                    DrawHealthBar(FeetCoords, HeadCoords, entity[i].health);
                if (enable_enemy_armorbar)
                    DrawArmorBar(FeetCoords, HeadCoords, entity[i].armor);


                if (debug_aimbot)
                {
                    // Triangle 1 XY
                    glLineWidth(1.0f);
                    DrawLine(screenCorners2[0], FeetCoords, triangleColor);
                    RenderText("90", fontBase, screenCorners2[2].X, screenCorners2[2].Y + 0.1);
                    DrawLine(FeetCoords, screenCorners2[2], triangleColor);
                    DrawLine(screenCorners2[2], screenCorners2[0], triangleColor);
                    RenderText(str.c_str(), fontBase, screenCorners2[0].X, screenCorners2[0].Y + 0.1);

                    //Triangle 2 Z
                    glLineWidth(1.0f);
                    DrawLine(screenCorners2[0], FeetCoords, triangleColor);

                    glLineWidth(1.0f);
                    DrawLine(FeetCoords, HeadCoords, triangleColor);

                    glLineWidth(1.0f);
                    DrawLine(HeadCoords, screenCorners2[0], triangleColor);
                    glLineWidth(1.0f);
                }

                glColor4f(1, 1, 1, 1);
#
                if(esp_show_names)
                    RenderText("Enemy", fontBase, HeadCoords.X, HeadCoords.Y);


                if (enable_aimbot)
                    CalculateNewAngles(Player, entity[i]);

                str = std::to_string(new_pitch);
                str1 = std::to_string(new_yaw);
            }
            else {
                switch (esp_mode)
                {
                case 1:
                    if (enable_team_esp)
                        DrawBox(FeetCoords, HeadCoords, color_team_box);
                    break;
                case 2:
                    if (allCornersValid && enable_team_esp)
                        DrawBox3D(boundingBoxesNdc, color_team_box);
                    break;
                case 3:
                    if (enable_team_esp)
                        DrawCornerOutline(FeetCoords, HeadCoords, color_team_box);
                    break;
                }

                if (enable_team_snaplines)
                    DrawSnapLines(FeetCoords, color_team_snaplines);
                if (enable_team_healthbar)
                    DrawHealthBar(FeetCoords, HeadCoords, entity[i].health);
                if (enable_team_armorbar)
                    DrawArmorBar(FeetCoords, HeadCoords, entity[i].armor);

                glColor4f(1,1,1,1);

                if (esp_show_names)
                    RenderText("Teammate", fontBase, HeadCoords.X, HeadCoords.Y);

            }

            Vec3 gizmo[3] =
            {
                {entity[i].Origin.X + 1, entity[i].Origin.Y, entity[i].Origin.Z},
                {entity[i].Origin.X, entity[i].Origin.Y - 1, entity[i].Origin.Z},
                {entity[i].Origin.X, entity[i].Origin.Y , entity[i].Origin.Z + 1.}
            };

            Vec2 gizmoNdc[3];

            Vec3 gizmoPlayer[3] =
            {
                {Player.Origin.X + 1, Player.Origin.Y, Player.Origin.Z},
                {Player.Origin.X, Player.Origin.Y - 1, Player.Origin.Z},
                {Player.Origin.X, Player.Origin.Y , Player.Origin.Z + 1.}
            };

            Vec2 gizmoPlayerNdc[3];
            
            for (int i = 0; i < 3; i++)
            {
                WorldToScreen(gizmo[i], gizmoNdc[i], Player.Matrix.Matrix);
            }

            for (int i = 0; i < 3; i++)
            {
                WorldToScreenPlayer(gizmoPlayer[i], gizmoPlayerNdc[i], Player.Matrix.Matrix);
            }

            if (debug_vectors)
            {
                glLineWidth(3.0f);

                float red[4] = { 1.0f, 0.0f, 0.0f, 1.0f };
                DrawLine(FeetCoords, gizmoNdc[0], red);
                RenderText("X", fontBase, gizmoNdc[0].X + 0.01, gizmoNdc[0].Y + 0.01);

                float green[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
                DrawLine(FeetCoords, gizmoNdc[1], green);
                RenderText("Y", fontBase, gizmoNdc[1].X + 0.01, gizmoNdc[1].Y + 0.01);

                float blue[4] = { 0.0f, 0.0f, 1.0f, 1.0f };
                DrawLine(FeetCoords, gizmoNdc[2], blue);
                RenderText("Z", fontBase, gizmoNdc[2].X + 0.01, gizmoNdc[2].Y + 0.01);

                glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
                RenderText("LAST POSTION", fontBase, playerFeetCoords.X, playerFeetCoords.Y - 0.05);


                glLineWidth(1.0f);
            }

            if (debug_vectors)
            {
                glLineWidth(3.0f);
                float red[4] = { 0.5f, 0.0f, 0.0f, 0.0f };
                DrawLine(playerFeetCoords, gizmoPlayerNdc[0], red);
                RenderText("X", fontBase, gizmoPlayerNdc[0].X + 0.01, gizmoPlayerNdc[0].Y + 0.01);

                float green[4] = { 0.0f, 0.5f, 0.0f, 0.0f };
                DrawLine(playerFeetCoords, gizmoPlayerNdc[1], green);
                RenderText("Y", fontBase, gizmoPlayerNdc[1].X + 0.01, gizmoPlayerNdc[1].Y + 0.01);

                float blue[4] = { 0.0f, 0.0f, 0.5f, 0.0f };
                DrawLine(playerFeetCoords, gizmoPlayerNdc[2], blue);
                RenderText("Z", fontBase, gizmoPlayerNdc[2].X + 0.01, gizmoPlayerNdc[2].Y + 0.01);
                glLineWidth(1.0f);
            }
        }

        glColor4f(1, 1, 1, 1);
        RenderText(str.c_str(), fontBase, -0.8f, 0.9f);
        RenderText("Calculated Pitch: ", fontBase, -0.99f, 0.9f);

        RenderText("Calculated Yaw: ", fontBase, -0.99f, 0.8f);
        RenderText(str1.c_str(), fontBase, -0.8f, 0.8f);

        if (activatedLastPos)
        {
            glColor4f(0,1,0,1);
        }
        else
        {
            glColor4f(1, 0, 0, 1);
        }
        RenderText("Save Last Pos: Backspace", fontBase, -0.99f, 0.7f);


        Memory::WPM<int>(Player.baseaddress + offsets::mag_assaultRifle, 1337);
        Memory::WPM<int>(Player.baseaddress + offsets::health, 1337);

        if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && enable_aimbot && nearest_enemy != -1.0f)
        {
            Memory::WPM<Vec3>(Player.baseaddress + offsets::yaw, { new_yaw, new_pitch, 0 });
        }

		if (GetAsyncKeyState(VK_BACK) & 1)
		{
            activatedLastPos = !activatedLastPos;

            if (activatedLastPos)
            {
                PreviousPosition = Player.Origin;
            }
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
