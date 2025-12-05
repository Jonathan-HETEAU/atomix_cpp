#include "atomixRaylib.hpp"

namespace Atomix
{

    void AtomixRaylib::updateInput(float delta)
    {
        auto position = GetScreenToWorld2D(GetMousePosition(), panelLevel.camera);
        currentMousePosition = {(int)position.x / CASE_SIZE, (int)position.y / CASE_SIZE};

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            partie->onClick(currentMousePosition);
        }
        if (IsKeyPressed(KEY_TAB))
        {
            partie->onSwitchAtom();
        }
        if (IsKeyPressed(KEY_UP))
        {
            partie->onMoveAtom(UP);
        }
        if (IsKeyPressed(KEY_DOWN))
        {
            partie->onMoveAtom(DOWN);
        }
        if (IsKeyPressed(KEY_LEFT))
        {
            partie->onMoveAtom(LEFT);
        }
        if (IsKeyPressed(KEY_RIGHT))
        {
            partie->onMoveAtom(RIGHT);
        }
        if( IsKeyPressed(KEY_SPACE)){
            partie->onUndo();
        }
    }

    void AtomixRaylib::drawLevel()
    {
        BeginTextureMode(panelLevel.render);
        ClearBackground(BLACK);
        BeginMode2D(panelLevel.camera);
        partie->draw(*this);
        EndMode2D();
        EndTextureMode();
        DrawTextureRec(panelLevel.render.texture, {0, 0, (float)panelLevel.render.texture.width, (float)-panelLevel.render.texture.height}, {(float)panelLevel.position.x, (float)panelLevel.position.y}, WHITE);
    }

    void AtomixRaylib::drawMolecul()
    {
        BeginTextureMode(panelMolecul.render);
        ClearBackground(GREEN);
        BeginMode2D(panelMolecul.camera);
        for (int atomIndex = 0; atomIndex < level.nbrAtoms; atomIndex++)
        {
            AtomData &atom = level.solution[atomIndex];
            Color color = {
                (unsigned char)((atom.value >> 24) & 0xFF), // R
                (unsigned char)((atom.value >> 16) & 0xFF), // G
                (unsigned char)((atom.value >> 8) & 0xFF),  // B
                (unsigned char)(atom.value | 0xFF)          // A
            };

            DrawCircle(atom.position.x * CASE_SIZE + DEMI_CASE_SIZE, atom.position.y * CASE_SIZE + DEMI_CASE_SIZE, DEMI_CASE_SIZE, color);
            DrawCircle(atom.position.x * CASE_SIZE + DEMI_CASE_SIZE, atom.position.y * CASE_SIZE + DEMI_CASE_SIZE, DEMI_CASE_SIZE * 0.8, ColorBrightness(color, 0.8));
        }
        EndMode2D();
        EndTextureMode();
        DrawTexturePro(
            panelMolecul.render.texture,
            {0, 0, (float)panelMolecul.render.texture.width, (float)-panelMolecul.render.texture.height},
            {(float)panelMolecul.position.x, (float)panelMolecul.position.y, (float)panelMolecul.width, (float)panelMolecul.height},
            {0, 0},0, WHITE);
    }

    void AtomixRaylib::draw(float delta)
    {
        drawLevel();
        drawMolecul();
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

        Color color = {
            (unsigned char)((atom.data.value >> 24) & 0xFF), // R
            (unsigned char)((atom.data.value >> 16) & 0xFF), // G
            (unsigned char)((atom.data.value >> 8) & 0xFF),  // B
            (unsigned char)(atom.data.value | 0xFF)          // A
        };

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
        // DrawRectangle(0, 0, width * CASE_SIZE, height * CASE_SIZE, WHITE);
        if (!partie->isWin())
        {

            for (int x = 1; x < width - 1; x++)
            {
                for (int y = 1; y < height - 1; y++)
                {
                    if (map[y][x])
                    {
                        DrawRectangle((float)(x * CASE_SIZE), (float)(y * CASE_SIZE), CASE_SIZE, CASE_SIZE, WHITE);
                    }
                }
            }
        }
    }

}
