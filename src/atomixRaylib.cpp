#include "atomixRaylib.hpp"

namespace Atomix
{

    void AtomixRaylib::updateInput(float delta)
    {
        auto position = GetScreenToWorld2D(GetMousePosition(), camera);
        currentMousePosition = {(int)position.x / CASE_SIZE, (int)position.y / CASE_SIZE};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            partie->onClick(currentMousePosition);
        }
    }

    void AtomixRaylib::draw(float delta)
    {

        BeginMode2D(camera);
        partie->draw(*this);
        EndMode2D();
    }

    void AtomixRaylib::drawDirection(Direction direction, Position position, Color color)
    {
        float x = position.x + (DIRECTION_VECTOR.at(direction).x * -0.2);
        float y = position.y + (DIRECTION_VECTOR.at(direction).y * -0.2);
        Color in = WHITE;
        if (position == currentMousePosition)
        {
            in = color;
            color = WHITE;
        }
        DrawTextureEx(AssetsDirection[direction], {(x - 0.05f) * CASE_SIZE, (y - 0.05f) * CASE_SIZE}, 0, 1.1, in);
        DrawTexture(AssetsDirection[direction], x * CASE_SIZE, y * CASE_SIZE, color);
    }

    void AtomixRaylib::drawAtom(Atom &atom)
    {   
        
        Color color = ColorFromNormalized({((atom.data.value * 28) % 255) / 255.f, ((atom.data.value * 100) % 255) / 255.f, ((atom.data.value * 40) % 255) / 255.f, 1.f});
        if (atom.isSelected())
        {
            for (Direction dir : atom.movePosible)
            {
                drawDirection(dir, DIRECTION_VECTOR.at(dir) + atom.data.position, color);
            }
            color = ColorBrightness(color, 0.5);
        }
        DrawCircle(atom.data.position.x * CASE_SIZE + DEMI_CASE_SIZE, atom.data.position.y * CASE_SIZE + DEMI_CASE_SIZE, ATOME_SIZE, color);
        DrawCircle(atom.data.position.x * CASE_SIZE + DEMI_CASE_SIZE, atom.data.position.y * CASE_SIZE + DEMI_CASE_SIZE, ATOME_SIZE * 0.8, ColorBrightness(color, 0.8));
    }

    void AtomixRaylib::drawMap(int width, int height, bool **map)
    {
        //DrawRectangle(0, 0, width * CASE_SIZE, height * CASE_SIZE, WHITE);
        if(!partie->isWin()){
            
            for (int x = 1; x < width - 1; x++)
            {
                for (int y = 1; y < height - 1; y++)
                {
                    if (map[y][x])
                    {
                        DrawRectangle((float)(x * CASE_SIZE), (float)(y * CASE_SIZE),CASE_SIZE,CASE_SIZE, WHITE);
                    }
                }
            }
        }
    }

}
