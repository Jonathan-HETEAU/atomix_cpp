#include "atomixRaylib.hpp"


namespace Atomix
{
    
    void AtomixRaylib::updateInput(float delta)
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            auto position = GetScreenToWorld2D(GetMousePosition(), camera);
            partie->onClick({(int)position.x / CASE_SIZE, (int)position.y / CASE_SIZE});
        }
    }

    void AtomixRaylib::draw(float delta)
    {

        BeginMode2D(camera);
        partie->draw(*this);
        EndMode2D();
    }

    void AtomixRaylib::drawAtom(Atom &atom)
    {
        DrawCircle(atom.position.x * CASE_SIZE + DEMI_CASE_SIZE, atom.position.y * CASE_SIZE + DEMI_CASE_SIZE, ATOME_SIZE, GREEN);
        DrawCircle(atom.position.x * CASE_SIZE + DEMI_CASE_SIZE, atom.position.y * CASE_SIZE + DEMI_CASE_SIZE, ATOME_SIZE * 0.8, ColorBrightness(GREEN, 0.8));
    }

    void AtomixRaylib::drawMap(int width, int height, bool **map)
    {
        DrawRectangle(0, 0, width * CASE_SIZE, height * CASE_SIZE, WHITE);
        for (int x = 1; x < width - 1; x++)
        {
            for (int y = 1; y < height - 1; y++)
            {
                if (map[y][x])
                {
                    Texture2D texture = GetTextureByPosition(map, x, y);
                    DrawTexture(texture, (float)(x * CASE_SIZE), (float)(y * CASE_SIZE), WHITE);
                }
            }
        }
    }

}
