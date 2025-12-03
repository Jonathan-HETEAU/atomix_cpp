#pragma once
#include "atomix.hpp"
#include "partie.hpp"
#include "painter.hpp"
#include <raylib.h>
#include <map>
#include <string>

namespace Atomix
{

    constexpr int CASE_SIZE = 128;
    constexpr int DEMI_CASE_SIZE = CASE_SIZE / 2;
    constexpr int ATOME_SIZE = DEMI_CASE_SIZE * 0.8;

    const std::string ASSETS_PATH_BY_SIZE = std::string(ASSETS_PATH) + TextFormat("%d/",CASE_SIZE);

    class AtomixRaylib : public Painter
    {
    private:
        inline static Texture2D Assets[16] = {};
        inline static Texture2D AssetsDirection[4] = {};

        Camera2D camera;
        Partie *partie;
        float currentDeltat = 0.f;
        Position currentMousePosition = {0,0};

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

    public:
        static void InitAssets()
        {   
            AssetsDirection[UP] = LoadTexture((ASSETS_PATH_BY_SIZE+ "Up.png").c_str());
            AssetsDirection[RIGHT] = LoadTexture((ASSETS_PATH_BY_SIZE+ "Right.png").c_str());
            AssetsDirection[DOWN] = LoadTexture((ASSETS_PATH_BY_SIZE+ "Down.png").c_str());
            AssetsDirection[LEFT] = LoadTexture((ASSETS_PATH_BY_SIZE+ "Left.png").c_str()); 

            for (int i = 1; i < 16; i++)
            {
                Assets[i] = LoadTexture((ASSETS_PATH_BY_SIZE+ TextFormat("%d.png",  i)).c_str());
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
        void drawDirection(Direction direction,Position position , Color color);
    };

}