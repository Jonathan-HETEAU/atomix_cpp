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

    const std::string ASSETS_PATH_BY_SIZE = std::string(ASSETS_PATH) + TextFormat("images/%d/", CASE_SIZE);

    struct Panel
    {

        int width;
        int height;
        Position position;
        RenderTexture2D render;
        Camera2D camera;
    };

    class AtomixRaylib : public Painter, public PartieObserver
    {
    private:
        inline static Texture2D Assets[16] = {};
        inline static Texture2D AssetsDirection[4] = {};

        Partie *partie;

        LevelData &level;

        Panel panelLevel;
        Panel panelMolecul;

        float currentDeltat = 0.f;
        Position currentMousePosition = {0, 0};

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
            AssetsDirection[UP] = LoadTexture((ASSETS_PATH_BY_SIZE + "Up.png").c_str());
            AssetsDirection[RIGHT] = LoadTexture((ASSETS_PATH_BY_SIZE + "Right.png").c_str());
            AssetsDirection[DOWN] = LoadTexture((ASSETS_PATH_BY_SIZE + "Down.png").c_str());
            AssetsDirection[LEFT] = LoadTexture((ASSETS_PATH_BY_SIZE + "Left.png").c_str());

            for (int i = 1; i < 16; i++)
            {
                Assets[i] = LoadTexture((ASSETS_PATH_BY_SIZE + TextFormat("%d.png", i)).c_str());
            }
        }

        AtomixRaylib(LevelData &level)
            : level(level), partie(new Partie(level)), panelLevel({GetScreenWidth() - 200,
                                                                   GetScreenHeight(),
                                                                   {0, 0},
                                                                   LoadRenderTexture(GetScreenWidth() - 200, GetScreenHeight()),
                                                                   {{(GetScreenWidth() - 200) / 2.0f, GetScreenHeight() / 2.0f},
                                                                    {(float)level.width * DEMI_CASE_SIZE, (float)level.height * DEMI_CASE_SIZE},
                                                                    0.f,
                                                                    std::min(GetScreenHeight() / (float)(level.height * CASE_SIZE), (GetScreenWidth() - 200) / (float)(level.width * CASE_SIZE))}}),
              panelMolecul({200,
                            200,
                            {GetScreenWidth() - 200, GetScreenHeight() - 200},
                            LoadRenderTexture(level.width * CASE_SIZE, level.height * CASE_SIZE),
                            {{0, 0},
                             {0, 0},
                             0.f,
                             1}})
        {
            Position min{level.width, level.height};
            Position max{0, 0};
            for (int atomIndex = 0; atomIndex < level.nbrAtoms; atomIndex++)
            {
                min.x = std::min(min.x, level.solution[atomIndex].position.x);
                min.y = std::min(min.y, level.solution[atomIndex].position.y);
                max.x = std::max(max.x, level.solution[atomIndex].position.x);
                max.y = std::max(max.y, level.solution[atomIndex].position.y);
            }
            float decal = std::max(max.x - min.x,max.y - min.y)+1;
            panelMolecul.camera.offset={(level.width * CASE_SIZE)/2.f, (level.height * CASE_SIZE)/2.f};
            panelMolecul.camera.target={(float)(max.x + min.x + 1) * DEMI_CASE_SIZE ,(float)(max.y + min.y +1) * DEMI_CASE_SIZE};
            panelMolecul.camera.zoom = (std::min(level.height, level.width)/(decal));

        };

        ~AtomixRaylib()
        {
            delete partie; // Don't forget to clean up!
        }

        void updateInput(float delta);

        void draw(float delta);
        void drawLevel();
        void drawMolecul();
        void drawAtom(Atom &atom);
        void drawMap(int width, int height, bool **map);
        void drawDirection(Direction direction, Position position, Color color);
        void onWin();
        void onAtomSelected(Atom &selected);
        void onAtomUnselected(Atom &unselected);
        void onAtomMove(Atom &atom, Position &origin, Position &dest);
    };

}