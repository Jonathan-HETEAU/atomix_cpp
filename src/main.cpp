#include "raylib.h"

#include "imgui.h"
#include "rlImGui.h"
#include "rlImGuiColors.h"

#include "levelLoader.hpp"
#include "atomixRaylib.hpp"

using namespace Atomix;

int main()
{
    InitWindow(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()), "Atomix");
    ToggleFullscreen();
    SetTargetFPS(60);

    AtomixRaylib::InitAssets();

    // Charger depuis JSON
    LevelData level = loadLevel(1);
    AtomixRaylib atomix(level);

    rlImGuiSetup(true);
    while (!WindowShouldClose())
    {
        atomix.updateInput(GetFrameTime());
        BeginDrawing();
        ClearBackground(WHITE);
        atomix.draw(GetFrameTime());
        rlImGuiBegin();
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                    CloseWindow();

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Window"))
            {

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
        rlImGuiEnd();
        EndDrawing();
    }

    // inside your game loop, between BeginDrawing() and EndDrawing()

    rlImGuiShutdown();
    freeLevelData(level);
    CloseWindow();
    return 0;
}