#include "raylib.h"
#include "rlImGui.h"
#include "imgui.h"
#include "flecs.h"
#include <string>

typedef Color Atom;
typedef struct Vector2 Position;

constexpr int CASE_SIZE = 64;
constexpr int DEMI_CASE_SIZE = CASE_SIZE / 2;
constexpr int ATOME_SIZE = DEMI_CASE_SIZE * 0.8;

struct Player {};
struct Game
{
    Vector2 size;
    bool **maps;
};

struct Click {
    int x;
    int y;
};

constexpr Vector2 DIRECTION[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}};

static Texture2D Assets [16];

Texture2D getTextureByPosition(bool **maps, int x, int y)
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

void drawMaps(Game game)
{
    DrawRectangle(0,0,game.size.x*CASE_SIZE,game.size.y*CASE_SIZE,WHITE);
    for (int x = 1; x < game.size.x - 1; x++)
    {
        for (int y = 1; y < game.size.y - 1; y++)
        {
            if (game.maps[y][x])
            {
                Texture2D texture = getTextureByPosition(game.maps, x, y);
                DrawTexture(texture,(float)(x * CASE_SIZE), (float)(y * CASE_SIZE),WHITE);
            }
        }
    }
}

void DrawAtome(const Atom &atom, const Position &position)
{
    DrawCircle(position.x * CASE_SIZE + DEMI_CASE_SIZE, position.y * CASE_SIZE + DEMI_CASE_SIZE, ATOME_SIZE, atom);
    DrawCircle(position.x * CASE_SIZE + DEMI_CASE_SIZE, position.y * CASE_SIZE + DEMI_CASE_SIZE, ATOME_SIZE*0.8, ColorBrightness(atom,0.8));
}

void CameraRenderSystem(flecs::iter &iter, size_t, const Camera2D &camera)
{

    auto game = iter.world().get<Game>();
    auto atoms = iter.world().query<const Atom, const Position>();
    BeginMode2D(camera);
    drawMaps(game);
    atoms.each([](const Atom &atom, const Position &position)
               { DrawAtome(atom, position); });
    EndMode2D();
}


void inputSystem(flecs::iter &iter ,size_t e , const Camera2D &camera , const Player){

    auto game = iter.world().get<Game>();

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
        auto position = GetScreenToWorld2D(GetMousePosition(),camera);
        if ( position.x < 0 || position.y < 0 || position.x >=  game.size.x * CASE_SIZE || position.y >= game.size.y * CASE_SIZE )
            return;

        Click click = {position.x/CASE_SIZE, position.y/CASE_SIZE};

        iter.entity(e).set(click);

    }
}

void clickSystem(flecs::iter &iter ,size_t e , Click &click){
    
    auto atoms = iter.world().query< Atom, const Position>();
    
    atoms.each([&click](flecs::iter &iter ,size_t e, Atom &atom, const Position &position){
        if(click.x == position.x && click.y == position.y){
            atom = BLACK;
        }
    });
    

}

Game initGame()
{

    Vector2 size = {8, 6};
    bool **maps = new bool *[(int)size.y];

    maps[0] = new bool[](false,false, false, false, false, false, false, false);
    maps[1] = new bool[](false,false, true, true, true, false, false, false);
    maps[2] = new bool[](false,true, true, true, true, true, true, false);
    maps[3] = new bool[](false,false, true, true, true, false, false, false);
    maps[4] = new bool[](false,true, true, false, true, true, true, false);
    maps[5] = new bool[](false,false, false, false, false, false, false, false);

    return {size, maps};
}

void initAssets(){

    for(int i = 1 ; i < 16 ; i++){
        
        Assets[i] = LoadTexture((std::string(ASSETS_PATH) + TextFormat("/%d/%d.png",CASE_SIZE, i)).c_str());
    }

}

flecs::world initWorld()
{
    flecs::world world;

    Game game = initGame();

    world.set<Game>(game);

    world.entity()
        .add<Player>()
        .set<Camera2D>({{(GetScreenWidth() / 2.0f), (GetScreenHeight() / 2.0f)}, {game.size.x * DEMI_CASE_SIZE, game.size.y * DEMI_CASE_SIZE}, 0,GetScreenHeight()/(game.size.y*CASE_SIZE) });

    world.entity()
        .set<Atom>(RED)
        .set<Position>({3, 2});

    world.entity()
        .set<Atom>(BLUE)
        .set<Position>({1, 3});

    
    world.system<const Camera2D,const Player>().each(inputSystem);
    world.system<Click>().each(clickSystem);

    return world;
}

int main()
{
    InitWindow(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()), "Atomix - Hello World");
    ToggleFullscreen();
    SetTargetFPS(60);
    rlImGuiSetup(true);
    initAssets();
    flecs::world world = initWorld();
    flecs::query<const Camera2D> queryCamera = world.query<const Camera2D>("CameraRenderSystem");

    while (!WindowShouldClose())
    {
        world.progress(GetFrameTime());
        BeginDrawing();
        ClearBackground(BLACK);
        queryCamera.each(CameraRenderSystem);
        EndDrawing();
    }

    rlImGuiShutdown();
    CloseWindow();

    return 0;
}