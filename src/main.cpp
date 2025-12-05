#include "raylib.h"
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
    
    while (!WindowShouldClose())
    {   
        atomix.updateInput(GetFrameTime());
        BeginDrawing();
        ClearBackground(WHITE);
        atomix.draw(GetFrameTime());
        EndDrawing();
    }

    freeLevelData(level);
    CloseWindow();
    return 0;
}