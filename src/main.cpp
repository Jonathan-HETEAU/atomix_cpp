#include "raylib.h"
#include <string>
#include <algorithm>
#include <list>
#include "atomix.hpp"
#include "atomixRaylib.hpp"

using namespace Atomix;

LevelData createLevel()
{
    Atomix::LevelData level;
    level.height = 5;
    level.width = 6;
    bool **maps = new bool *[5];

    maps[0] = new bool[](0, 0, 0, 0, 0, 0);
    maps[1] = new bool[](0, 1, 0, 0, 0, 0);
    maps[2] = new bool[](0, 1, 1, 1, 1, 0);
    maps[3] = new bool[](0, 1, 1, 1, 1, 0);
    maps[4] = new bool[](0, 0, 0, 0, 0, 0);

    level.map = maps;

    level.atoms    =  new Atomix::AtomData[]({0, {1, 1}}, {1, {4, 2}});
    level.solution = new Atomix::AtomData[]({0, {0,0}}, {1, {1,0}});
    level.nbrAtoms = 2;
    return level;
}



int main()
{
    InitWindow(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()), "Atomix - Hello World");
    ToggleFullscreen();
    SetTargetFPS(60);
    Atomix::AtomixRaylib::InitAssets();
    LevelData level = createLevel();
    AtomixRaylib atomix = AtomixRaylib(level);
    while (!WindowShouldClose())
    {   
        atomix.updateInput(GetFrameTime());
        BeginDrawing();
        ClearBackground(BLACK);
        atomix.draw(GetFrameTime());
        EndDrawing();
    }

    CloseWindow();

    return 0;
}