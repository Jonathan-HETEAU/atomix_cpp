#pragma once
#include "atomix.hpp"
#include "partie.hpp"
#include "painter.hpp"
#include <raylib.h>
#include <map>
#include <string>

namespace Atomix
{

    constexpr int CASE_SIZE = 64;
    constexpr int DEMI_CASE_SIZE = CASE_SIZE / 2;
    constexpr int ATOME_SIZE = DEMI_CASE_SIZE * 0.8;

    class AtomixRaylib : public Painter
    {
    private:
        inline static Texture2D Assets[16] = {};
        static Texture2D GetTextureByPosition(bool **maps, int x, int y)
        {
            int value = 0;
            if (maps[y - 1][x])
                value += 8;
            if (maps[y][x + 1])
                value += 4;
            if (maps[y + 1][x])
                value += 2;
            if (maps[y][x - 1])
                value += 1;
            return Assets[value];
        }
        Camera2D camera;
        Partie *partie;

    public:
        static void InitAssets()
        {
            for (int i = 1; i < 16; i++)
            {
                Assets[i] = LoadTexture((std::string(ASSETS_PATH) + TextFormat("/%d/%d.png", CASE_SIZE, i)).c_str());
            }
        }
        
        
        AtomixRaylib(LevelData &level)
            : partie(new Partie(level)), camera{
                                             {GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f},
                                             {(float)level.width * DEMI_CASE_SIZE, (float)level.height * DEMI_CASE_SIZE},
                                             0.f,
                                             GetScreenHeight() / (float)(level.height * CASE_SIZE)} {};

        ~AtomixRaylib()
        {
            delete partie; // Don't forget to clean up!
        }

        void updateInput(float delta);
        void draw(float delta);
        void drawAtom(Atom &atom);
        void drawMap(int width, int height, bool **map);
    };

}